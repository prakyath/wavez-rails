/*
 * twemproxy - A fast and lightweight proxy for memcached protocol.
 * Copyright (C) 2011 Twitter, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdlib>
#include <cstring>
#include <oxt/macros.hpp>
#include <oxt/thread.hpp>
#include <cstring>
#include <algorithm>
#include <MemoryKit/mbuf.h>

namespace Passenger {
namespace MemoryKit {

//#define MBUF_DEBUG


static void
_mbuf_block_mark_as_active(struct mbuf_pool *pool, struct mbuf_block *mbuf_block)
{
	STAILQ_NEXT(mbuf_block, next) = NULL;
	#ifdef MBUF_ENABLE_DEBUGGING
		TAILQ_INSERT_HEAD(&pool->active_mbuf_blockq, mbuf_block, active_q);
	#endif
	#ifdef MBUF_ENABLE_BACKTRACES
		mbuf_block->backtrace = strdup(oxt::thread::current_backtrace().c_str());
	#endif
	pool->nactive_mbuf_blockq++;
}

static struct mbuf_block *
_mbuf_block_init(struct mbuf_pool *pool, char *buf, size_t block_offset)
{
	struct mbuf_block *mbuf_block;

	/*
	 * There are two types of mbuf_blocks: normal ones and standalone ones.
	 *
	 * ## Normal mbuf_blocks
	 *
	 * mbuf_block header is at the tail end of the mbuf_block. The data
	 * precedes the header. This enables us to catch buffer overrun early
	 * by asserting on the magic value during get or put operations.
	 * All normal mbuf_blocks in a pool have the same mbuf_block_offset,
	 * allowing them to be reused through a freelist.
	 *
	 *   <------------ pool->mbuf_block_chunk_size -------------->
	 *   +-------------------------------------------------------+
	 *   |       mbuf_block data          |  mbuf_block header   |
	 *   |                                |                      |
	 *   |  (pool->mbuf_block_offset)     | (struct mbuf_block)  |
	 *   +-------------------------------------------------------+
	 *   ^                                ^
	 *   |                                |
	 *   \                                |\
	 * block->start                       | block->end (one byte past valid bound)
	 *                                    \
	 *                                    block
	 *
	 * ## Standalone mbuf_blocks
	 *
	 * Standalone mbuf_blocks are like normal ones, but can contain
	 * arbitrarily-sized data. Different standalone mbuf_blocks in a pool
	 * can have different data sizes. They cannot be reused through the
	 * freelist. The fact that the 'offset' field in the header is set to
	 * a non-zero is an indication that it is standalone.
	 *
	 *   <------------- offset + MBUF_BLOCK_HSIZE --------------->
	 *   +-------------------------------------------------------+
	 *   |       mbuf_block data          |  mbuf_block header   |
	 *   |                                |                      |
	 *   |           (offset)             | (struct mbuf_block)  |
	 *   +-------------------------------------------------------+
	 *   ^                                ^
	 *   |                                |
	 *   \                                |\
	 * block->start                       | block->end (one byte past valid bound)
	 *                                    \
	 *                                    block
	 */
	mbuf_block = (struct mbuf_block *)(buf + block_offset);
	mbuf_block->magic = MBUF_BLOCK_MAGIC;
	mbuf_block->pool  = pool;
	mbuf_block->refcount = 1;
	mbuf_block->offset = 0;

	_mbuf_block_mark_as_active(pool, mbuf_block);
	return mbuf_block;
}

static struct mbuf_block *
_mbuf_block_get(struct mbuf_pool *pool)
{
	struct mbuf_block *mbuf_block;
	char *buf;

	if (!STAILQ_EMPTY(&pool->free_mbuf_blockq)) {
		assert(pool->nfree_mbuf_blockq > 0);

		mbuf_block = STAILQ_FIRST(&pool->free_mbuf_blockq);
		pool->nfree_mbuf_blockq--;
		STAILQ_REMOVE_HEAD(&pool->free_mbuf_blockq, next);

		assert(mbuf_block->magic == MBUF_BLOCK_MAGIC);
		_mbuf_block_mark_as_active(pool, mbuf_block);
		return mbuf_block;
	}

	buf = (char *) malloc(pool->mbuf_block_chunk_size);
	if (OXT_UNLIKELY(buf == NULL)) {
		return NULL;
	}

	return _mbuf_block_init(pool, buf, pool->mbuf_block_offset);
}

struct mbuf_block *
mbuf_block_get(struct mbuf_pool *pool)
{
	struct mbuf_block *mbuf_block;
	char *buf;

	mbuf_block = _mbuf_block_get(pool);
	if (OXT_UNLIKELY(mbuf_block == NULL)) {
		return NULL;
	}

	buf = (char *)mbuf_block - pool->mbuf_block_offset;
	mbuf_block->start = buf;
	mbuf_block->end = buf + pool->mbuf_block_offset;

	assert(mbuf_block->end - mbuf_block->start == (int)pool->mbuf_block_offset);
	assert(mbuf_block->start < mbuf_block->end);

	#ifdef MBUF_DEBUG
		printf("[%p] mbuf_block get %p\n", oxt::thread_signature, mbuf_block);
	#endif

	return mbuf_block;
}

struct mbuf_block *
mbuf_block_new_standalone(struct mbuf_pool *pool, size_t size)
{
	struct mbuf_block *mbuf_block;
	size_t block_offset;
	char *buf;

	block_offset = std::max<size_t>(size, mbuf_pool_data_size(pool));
	buf = (char *) malloc(MBUF_BLOCK_HSIZE + block_offset);
	if (OXT_UNLIKELY(buf == NULL)) {
		return NULL;
	}

	mbuf_block = _mbuf_block_init(pool, buf, block_offset);
	mbuf_block->start = buf;
	mbuf_block->end = buf + size;
	mbuf_block->offset = block_offset;

	assert(mbuf_block->end - mbuf_block->start == (int)size);
	assert(mbuf_block->start < mbuf_block->end);

	#ifdef MBUF_DEBUG
		printf("[%p] mbuf_block new standalone %p\n", oxt::thread_signature, mbuf_block);
	#endif

	return mbuf_block;
}

static void
mbuf_block_free(struct mbuf_block *mbuf_block)
{
	char *buf;

	#ifdef MBUF_DEBUG
		printf("[%p] mbuf_block free %p\n", oxt::thread_signature, mbuf_block);
	#endif

	assert(STAILQ_NEXT(mbuf_block, next) == NULL);
	assert(mbuf_block->magic == MBUF_BLOCK_MAGIC);

	#ifdef MBUF_ENABLE_DEBUGGING
		TAILQ_REMOVE(&mbuf_block->pool->active_mbuf_blockq, mbuf_block, active_q);
	#endif
	#ifdef MBUF_ENABLE_BACKTRACES
		free(mbuf_block->backtrace);
	#endif

	if (mbuf_block->offset > 0) {
		buf = (char *) mbuf_block - mbuf_block->offset;
	} else {
		buf = (char *) mbuf_block - mbuf_block->pool->mbuf_block_offset;
	}
	free(buf);
}

void
mbuf_block_put(struct mbuf_block *mbuf_block)
{
	#ifdef MBUF_DEBUG
		printf("[%p] mbuf_block put %p\n", oxt::thread_signature, mbuf_block);
	#endif

	assert(STAILQ_NEXT(mbuf_block, next) == NULL);
	assert(mbuf_block->magic == MBUF_BLOCK_MAGIC);
	assert(mbuf_block->refcount == 0);
	assert(mbuf_block->pool->nactive_mbuf_blockq > 0);
	assert(mbuf_block->offset == 0);

	mbuf_block->refcount = 1;
	mbuf_block->pool->nfree_mbuf_blockq++;
	mbuf_block->pool->nactive_mbuf_blockq--;
	STAILQ_INSERT_HEAD(&mbuf_block->pool->free_mbuf_blockq, mbuf_block, next);

	#ifdef MBUF_ENABLE_DEBUGGING
		TAILQ_REMOVE(&mbuf_block->pool->active_mbuf_blockq, mbuf_block, active_q);
	#endif
}

/*
 * Remove mbuf_block from the mhdr Q
 */
static void
mbuf_block_remove(struct mhdr *mhdr, struct mbuf_block *mbuf_block)
{
	//log_debug(LOG_VVERB, "remove mbuf_block %p", mbuf_block);

	STAILQ_REMOVE(mhdr, mbuf_block, struct mbuf_block, next);
	STAILQ_NEXT(mbuf_block, next) = NULL;
}

void
mbuf_pool_init(struct mbuf_pool *pool)
{
	pool->nfree_mbuf_blockq = 0;
	pool->nactive_mbuf_blockq = 0;
	STAILQ_INIT(&pool->free_mbuf_blockq);

	#ifdef MBUF_ENABLE_DEBUGGING
		TAILQ_INIT(&pool->active_mbuf_blockq);
	#endif

	pool->mbuf_block_offset = pool->mbuf_block_chunk_size - MBUF_BLOCK_HSIZE;
}

void
mbuf_pool_deinit(struct mbuf_pool *pool)
{
	mbuf_pool_compact(pool);
}

/*
 * Return the maximum available space size for data in any mbuf_block. Mbuf cannot
 * contain more than 2^32 bytes (4G).
 */
size_t
mbuf_pool_data_size(struct mbuf_pool *pool)
{
	return pool->mbuf_block_offset;
}

unsigned int
mbuf_pool_compact(struct mbuf_pool *pool)
{
	unsigned int count = pool->nfree_mbuf_blockq;

	while (!STAILQ_EMPTY(&pool->free_mbuf_blockq)) {
		struct mbuf_block *mbuf_block = STAILQ_FIRST(&pool->free_mbuf_blockq);
		mbuf_block_remove(&pool->free_mbuf_blockq, mbuf_block);
		mbuf_block_free(mbuf_block);
		pool->nfree_mbuf_blockq--;
	}
	assert(pool->nfree_mbuf_blockq == 0);

	return count;
}


void
mbuf_block_ref(struct mbuf_block *mbuf_block)
{
	#ifdef MBUF_DEBUG
		printf("[%p] mbuf_block ref %p: %u -> %u\n",
			oxt::thread_signature, mbuf_block,
			mbuf_block->refcount, mbuf_block->refcount + 1);
	#endif
	#ifdef MBUF_ENABLE_BACKTRACES
		mbuf_block->backtrace = strdup(oxt::thread::current_backtrace().c_str());
	#endif
	mbuf_block->refcount++;
}

void
mbuf_block_unref(struct mbuf_block *mbuf_block)
{
	#ifdef MBUF_DEBUG
		printf("[%p] mbuf_block unref %p: %u -> %u\n",
			oxt::thread_signature, mbuf_block,
			mbuf_block->refcount, mbuf_block->refcount - 1);
	#endif
	assert(mbuf_block->refcount > 0);
	mbuf_block->refcount--;
	if (mbuf_block->refcount == 0) {
		if (mbuf_block->offset > 0) {
			mbuf_block->pool->nactive_mbuf_blockq--;
			mbuf_block_free(mbuf_block);
		} else {
			mbuf_block_put(mbuf_block);
		}
	}
}

mbuf
mbuf_block_subset(struct mbuf_block *mbuf_block, unsigned int start, unsigned int len)
{
	return mbuf(mbuf_block, start, len);
}

mbuf
mbuf_get(struct mbuf_pool *pool)
{
	struct mbuf_block *block = mbuf_block_get(pool);
	if (OXT_UNLIKELY(block == NULL)) {
		return mbuf();
	}

	assert(block->refcount == 1);
	block->refcount--;
	return mbuf(block, 0, block->end - block->start);
}

mbuf
mbuf_get_with_size(struct mbuf_pool *pool, size_t size)
{
	struct mbuf_block *block;
	if (size <= mbuf_pool_data_size(pool)) {
		block = mbuf_block_get(pool);
	} else {
		block = mbuf_block_new_standalone(pool, size);
	}
	if (OXT_UNLIKELY(block == NULL)) {
		return mbuf();
	}

	assert(block->refcount == 1);
	block->refcount--;
	return mbuf(block, 0, size);
}


template<typename Address>
static Address clamp(Address value, Address min, Address max) {
	return std::max(std::min(value, max), min);
}

void
mbuf::initialize_with_block(unsigned int start, unsigned int len) {
	this->start = clamp<char *>(
		mbuf_block->start + start,
		mbuf_block->start,
		mbuf_block->end);
	this->end = clamp<char *>(
		mbuf_block->start + start + len,
		mbuf_block->start,
		mbuf_block->end);
	if (mbuf_block != NULL) {
		mbuf_block_ref(mbuf_block);
	}
}

void
mbuf::initialize_with_mbuf(const mbuf &mbuf, unsigned int start, unsigned int len) {
	mbuf_block = mbuf.mbuf_block;
	this->start = clamp<char *>(
		mbuf.start + start,
		mbuf.start,
		mbuf.end);
	this->end = clamp<char *>(
		mbuf.start + start + len,
		mbuf.start,
		mbuf.end);
	if (mbuf.mbuf_block != NULL) {
		mbuf_block_ref(mbuf.mbuf_block);
	}
}


} // namespace MemoryKit
} // namespace Passenger
