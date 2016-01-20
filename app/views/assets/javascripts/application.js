// This is a manifest file that'll be compiled into application.js, which will include all the files
// listed below.
//
// Any JavaScript/Coffee file within this directory, lib/assets/javascripts, vendor/assets/javascripts,
// or any plugin's vendor/assets/javascripts directory can be referenced here using a relative path.
//
// It's not advisable to add code directly here, but if you do, it'll appear at the bottom of the
// compiled file.
//
// Read Sprockets README (https://github.com/rails/sprockets#sprockets-directives) for details
// about supported directives.
//
//= require jquery
//= require jquery_ujs
//= require turbolinks
//= require_tree .
	
  <script>
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','http://www.google-analytics.com/analytics.js','ga');

  ga('create', 'UA-71672453-1', 'auto');
  ga('send', 'pageview');

  </script>



  


  <script src="assets/js/jquery-2.1.3.min.js"></script>
		<script type="text/javascript" src="assets/js/materialPreloader.js"></script>
		<script src="assets/js/bootstrap.js"></script>
		<script src="assets/js/jquery.actual.min.js"></script>
		<script src="assets/js/jquery.scrollTo.min.js"></script>
		<script src="assets/js/contact.js"></script>
		<script src="assets/js/script.js"></script>
		<script src="assets/js/smoothscroll.js"></script>
		<script src="assets/js/CSSPlugin.min.js"></script>
  		<script src="assets/js/EasePack.min.js"></script>
  		<script src="assets/js/wow.js"></script>
		<script src="assets/js/TweenLite.min.js"></script>
		

        

        <script type="text/javascript"> 
       
        preloader = new $.materialPreloader({
        position: 'top',
        height: '5px',
        col_1: '#159756',
        col_2: '#da4733',
        col_3: '#3b78e7',
        col_4: '#fdba2c',
        fadeIn: 200,
        fadeOut: 200
        });
              
        preloader.on();
         
        $( window ).load(function() { 

        preloader.off();
        jQuery("#preloader").fadeOut("slow");



       })
		

            jQuery(document).ready(function($){


		  	  
  
               $(window).scroll(function() {
		  		
		  		
				if ($(window).scrollTop() > 100 ){
		    
		 		$('.top-header').addClass('shows'); 
		    
		  		} else {
		    
		   	 	$('.top-header').removeClass('shows');
		    
		 		};   	
			});
/*....................home animations starts............................*/


    var windowheight = $(window).height();
  	var windowwidth = $(window).width();
  	
  	document.getElementById('homeframe').style.height = windowheight + "px";
  	document.getElementById('header').style.height = windowheight + "px";
  	document.getElementById('seacontainer').style.height = 0.15*windowheight + "px";
  	document.getElementById('boatcontainer').style.height = 0.3*windowheight + "px";
  	document.getElementById('boatcontainer').style.width = 0.4*windowheight + "px";
  	document.getElementById('boat').style.height = 0.3*windowheight + "px";
  	document.getElementById('boat').style.width = 0.4*windowheight + "px";
  	document.getElementById('anchorcontainer').style.height = 0.04*windowheight + "px";
  	document.getElementById('anchorcontainer').style.width = 0.025*windowheight + "px";
  	document.getElementById('anchor').style.height = 0.04*windowheight + "px";
  	document.getElementById('anchor').style.width = 0.025*windowheight + "px";
  	document.getElementById('departmentlogo').style.left = (0.95*windowwidth) + "px";
  	document.getElementById('iitmlogo').style.left = (0.95*windowwidth) + "px";
  	document.getElementById('facebooklogo').style.left = (0.95*windowwidth) + "px";
  	document.getElementById('waveztextcontainer').style.width = 0.5*windowheight + "px";
  	document.getElementById('waveztextcontainer').style.height = 0.125*windowheight + "px";

  	if (windowwidth<=1000) {
  		document.getElementById('schedule').style.height = "1500px";
  		document.getElementById('schedbutton').style.display = "none";
  	}

  	

  	 $( window ).load(function() {  

  	TweenLite.to(boatcontainer, 5.7, {left:(0.5*windowwidth), ease:Power1.easeOut});
  	TweenLite.to(sea, 5.7, {left:(-1*windowwidth), ease:Power1.easeOut});





  	TweenLite.to(boatcontainer, 1, {rotation:-5, transformOrigin:"50% 100%"});
  	TweenLite.to(anchorcontainer, 1, {rotation:10, transformOrigin:"50% 03%"});

  	TweenLite.to(boatcontainer, 1, {rotation:0, transformOrigin:"50% 100%", delay:1});
  	TweenLite.to(anchorcontainer, 1, {rotation:0, transformOrigin:"50% 03%", delay:1});

  	TweenLite.to(anchorcontainer, 0.7, {rotationY:180, delay:2});
  	TweenLite.to(boatcontainer, 0.7, {transformOrigin:"50% 100%",
  									  delay:2,
  									  ease:Power0.easeInOut});

  	TweenLite.to(boatcontainer, 1, {rotation:5, transformOrigin:"50% 100%", delay:2.7});
  	TweenLite.to(anchorcontainer, 1, {rotation:-10, transformOrigin:"50% 03%", delay:2.7});

  	TweenLite.to(boatcontainer, 1, {rotation:0, transformOrigin:"50% 100%", delay:3.7});
  	TweenLite.to(anchorcontainer, 1, {rotation:0, transformOrigin:"50% 03%", delay:3.7});

  	
  	TweenLite.to(anchorcontainer, 0.7, {rotationY:0, delay:4.7});




  	$("#boatcontainer").mouseenter(function(){
  		TweenLite.to(anchorcontainer, 0.7, {rotationY:360});
  		TweenLite.to(anchorcontainer, 0.7, {rotationY:0, delay:0.7});
  		TweenLite.to(anchorcontainer, 0.7, {rotation:10, transformOrigin:"50% 03%"});
  		TweenLite.to(anchorcontainer, 0.7, {rotation:-10, transformOrigin:"50% 03%", delay:0.7});
  		TweenLite.set(anchorcontainer, {rotation:0, delay:1.4});
  	});

  	$("#waveztextcontainer").mouseenter(function(){
  		TweenLite.fromTo(waveztextcontainer, 1, {autoAlpha:0, scale:0.7}, {autoAlpha:1, scale:1});
  	});


  	TweenLite.to(departmentlogo, 1.9, {
 									   top:(windowheight-40),
  									   scale:0.5,
  									   ease:Bounce.easeOut});
  	TweenLite.to(boatcontainer, 1.9, {scale:1.1, ease:Bounce.easeOut});
  	TweenLite.to(departmentlogo, 3.8, {rotation:-1080,
 									   left:(0.9*windowwidth-104), delay:1.9});







 	TweenLite.to(iitmlogo, 1.9, {
 							     top:(windowheight-40),
  							     scale:0.5,
  								 ease:Bounce.easeOut,
  								 delay:1.9});
  	TweenLite.to(boatcontainer, 1.9, {scale:1.2,
  									  ease:Bounce.easeOut,
  								   	  delay:1.9});
  	TweenLite.to(iitmlogo, 1.9, {rotation:-720,
 									   left:(0.9*windowwidth-52), delay:3.8});







 	TweenLite.to(facebooklogo, 1.9, {
 									 top:(windowheight-40),
  									 scale:0.5,
  									 ease:Bounce.easeOut,
  									 delay:3.8});
  	TweenLite.to(boatcontainer, 1.9, {scale:1.3, ease:Bounce.easeOut,
  									 delay:3.8});
  	TweenLite.to(facebooklogo, 1, {rotation:-360,
 									   left:(0.9*windowwidth), delay:5.7});


  	TweenLite.from(waveztextcontainer,  1, {scale:0.01, autoAlpha:0, delay:5.7});
  	



});

$(window).resize(function(){
	var windowheight = $(window).height();
  	var windowwidth = $(window).width();
  	
  	document.getElementById('homeframe').style.height = windowheight + "px";
  	document.getElementById('header').style.height = windowheight + "px";
  	document.getElementById('seacontainer').style.height = 0.15*windowheight + "px";
  	document.getElementById('boatcontainer').style.height = 0.3*windowheight + "px";
  	document.getElementById('boatcontainer').style.width = 0.4*windowheight + "px";
  	document.getElementById('boat').style.height = 0.3*windowheight + "px";
  	document.getElementById('boat').style.width = 0.4*windowheight + "px";
  	document.getElementById('anchorcontainer').style.height = 0.04*windowheight + "px";
  	document.getElementById('anchorcontainer').style.width = 0.025*windowheight + "px";
  	document.getElementById('anchor').style.height = 0.04*windowheight + "px";
  	document.getElementById('anchor').style.width = 0.025*windowheight + "px";
  	document.getElementById('departmentlogo').style.left = (0.95*windowwidth) + "px";
  	document.getElementById('iitmlogo').style.left = (0.95*windowwidth) + "px";
  	document.getElementById('facebooklogo').style.left = (0.95*windowwidth) + "px";
  	document.getElementById('waveztextcontainer').style.width = 0.5*windowheight + "px";
  	document.getElementById('waveztextcontainer').style.height = 0.125*windowheight + "px";

  	if (windowwidth<=1000) {
  		document.getElementById('schedule').style.height = "1500px";
  		document.getElementById('schedbutton').style.display = "none";
  	}

  	TweenLite.to(boatcontainer, 5.7, {left:(0.5*windowwidth), ease:Power1.easeOut});
  	TweenLite.to(sea, 5.7, {left:(-1*windowwidth), ease:Power1.easeOut});





  	TweenLite.to(boatcontainer, 1, {rotation:-5, transformOrigin:"50% 100%"});
  	TweenLite.to(anchorcontainer, 1, {rotation:10, transformOrigin:"50% 03%"});

  	TweenLite.to(boatcontainer, 1, {rotation:0, transformOrigin:"50% 100%", delay:1});
  	TweenLite.to(anchorcontainer, 1, {rotation:0, transformOrigin:"50% 03%", delay:1});

  	TweenLite.to(anchorcontainer, 0.7, {rotationY:180, delay:2});
  	TweenLite.to(boatcontainer, 0.7, {transformOrigin:"50% 100%",
  									  delay:2,
  									  ease:Power0.easeInOut});

  	TweenLite.to(boatcontainer, 1, {rotation:5, transformOrigin:"50% 100%", delay:2.7});
  	TweenLite.to(anchorcontainer, 1, {rotation:-10, transformOrigin:"50% 03%", delay:2.7});

  	TweenLite.to(boatcontainer, 1, {rotation:0, transformOrigin:"50% 100%", delay:3.7});
  	TweenLite.to(anchorcontainer, 1, {rotation:0, transformOrigin:"50% 03%", delay:3.7});

  	
  	TweenLite.to(anchorcontainer, 0.7, {rotationY:0, delay:4.7});




  	$("#boatcontainer").mouseenter(function(){
  		TweenLite.to(anchorcontainer, 0.7, {rotationY:360});
  		TweenLite.to(anchorcontainer, 0.7, {rotationY:0, delay:0.7});
  		TweenLite.to(anchorcontainer, 0.7, {rotation:10, transformOrigin:"50% 03%"});
  		TweenLite.to(anchorcontainer, 0.7, {rotation:-10, transformOrigin:"50% 03%", delay:0.7});
  		TweenLite.set(anchorcontainer, {rotation:0, delay:1.4});
  	});

  	$("#waveztextcontainer").mouseenter(function(){
  		TweenLite.fromTo(waveztextcontainer, 1, {autoAlpha:0, scale:0.7}, {autoAlpha:1, scale:1});
  	});


  	TweenLite.to(departmentlogo, 1.9, {
 									   top:(windowheight-40),
  									   scale:0.5,
  									   ease:Bounce.easeOut});
  	TweenLite.to(boatcontainer, 1.9, {scale:1.1, ease:Bounce.easeOut});
  	TweenLite.to(departmentlogo, 3.8, {rotation:-1080,
 									   left:(0.9*windowwidth-104), delay:1.9});







 	TweenLite.to(iitmlogo, 1.9, {
 							     top:(windowheight-40),
  							     scale:0.5,
  								 ease:Bounce.easeOut,
  								 delay:1.9});
  	TweenLite.to(boatcontainer, 1.9, {scale:1.2,
  									  ease:Bounce.easeOut,
  								   	  delay:1.9});
  	TweenLite.to(iitmlogo, 1.9, {rotation:-720,
 									   left:(0.9*windowwidth-52), delay:3.8});







 	TweenLite.to(facebooklogo, 1.9, {
 									 top:(windowheight-40),
  									 scale:0.5,
  									 ease:Bounce.easeOut,
  									 delay:3.8});
  	TweenLite.to(boatcontainer, 1.9, {scale:1.3, ease:Bounce.easeOut,
  									 delay:3.8});
  	TweenLite.to(facebooklogo, 1, {rotation:-360,
 									   left:(0.9*windowwidth), delay:5.7});


  	TweenLite.from(waveztextcontainer,  1, {scale:0.01, autoAlpha:0, delay:5.7});
  	

});



/*....................home animations ends............................*/
/* ...................events triggers starts........................... */

		







			$(".e").click(function(){
				$(this).closest("div").next().modal();
			});

				$(".elogin").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("div").hide(10);
					$(this).closest("div").next().children(".plogin").slideDown();
				});
				$(".eregister").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("div").hide(10);
					$(this).closest("div").next().children(".pregister").slideDown();
				});


				$(".eoverview").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("p").hide(10);
					$(this).closest("div").next().children(".poverview").slideDown();
				});
				$(".eprize").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("p").hide(10);
					$(this).closest("div").next().children(".pprize").slideDown();

				});
				$(".eprblmstmt").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("p").hide(10);
					$(this).closest("div").next().children(".pprblmstmt").slideDown();

				});

				$(".econtact").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("p").hide(10);
					$(this).closest("div").next().children(".pcontact").slideDown();

				});

				$(".erules").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("p").hide(10);
					$(this).closest("div").next().children(".prules").slideDown();

				});

				$(".eregister").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("p").hide(10);
					$(this).closest("div").next().children(".pregister").slideDown();

				});

				$(".eschedule").click(function(){
					$(this).closest("h4").children("a").removeClass("livetab");
					$(this).addClass("livetab");
					$(this).closest("div").next().children("p").hide(10);
					$(this).closest("div").next().children(".pschedule").slideDown();

				});
/* ...................events triggers ends........................... */

/* ...................schedule triggers starts........................... */

			



			var flag=1;
			var d= new Date();
			var ddate = d.getDate();
			var dmonth = d.getMonth();
			var n;
			var f;
			if (dmonth==11) {
				n = 100 - ddate;
			}
			else if (dmonth == 0){
				n = 69 - ddate;
			}
			else if (dmonth == 1){
				n = 38 - ddate;
			}
			else if (dmonth == 2){
				n = 9 - ddate;
			}
			else {
				n=-1;
			}

			f = ((n*2.5)+100) + "px";


  			var windowwidth1 = $(window).width();

			if(windowwidth1<1000)
			{
				$("#swater:not(.animated)").animate({
						height: f
					});
				if(n>0)
				{

					$(".dtg").text( n + " days to go...");
				}
				else
				{

					$(".dtg").text( "Sorry you are late");
				}


				$(".scheduletabd").prepend("<br><br>");
				

			}



			else
			{







			
			$("#schedule").hover(function(){

					if(1)

					{






				
					if(flag)
					{
					$("#swater:not(.animated)").animate({
						height: f
					});

					$({countNum:0}).animate(
						{countNum:n},
						{
							duration:3000,
							easing:"linear",
							step: function() {
								$(".dtg").text(Math.floor(this.countNum) + " days to go...");
											 },
							complete: function() {
								if(n>0)
								{

								$(".dtg").text( n + " days to go...");
								}
								else
								{

								$(".dtg").text( "Sorry you are late");
								}

							}

						}
						);

					$(".scheduletab > div > a").show();
					$(".scheduletab > div > a").animate({top : "+=350px",
														 width:"300px",
														 height:"300px",
														 borderRadius:"150px",
														 fontSize:"300%"}, 200);

					flag=0;
				 	}
					$("#swater").addClass("animated");


				}

				});

		}


		$(window).resize(function(){
			var windowwidth1 = $(window).width();

			if(windowwidth1<1000)
			{
				$("#swater:not(.animated)").animate({
						height: f
					});
				if(n>0)
				{

					$(".dtg").text( n + " days to go...");
				}
				else
				{

					$(".dtg").text( "Sorry you are late");
				}


				$(".scheduletabd").prepend("<br><br>");
				

			}



			else
			{







			
			$("#schedule").hover(function(){

					if(1)

					{


				
					if(flag)
					{
					$("#swater:not(.animated)").animate({
						height: f
					});

					$({countNum:0}).animate(
						{countNum:n},
						{
							duration:3000,
							easing:"linear",
							step: function() {
								$(".dtg").text(Math.floor(this.countNum) + " days to go...");
											 },
							complete: function() {
								if(n>0)
								{

								$(".dtg").text( n + " days to go...");
								}
								else
								{

								$(".dtg").text( "Sorry you are late");
								}

							}

						}
						);

					$(".scheduletab > div > a").show();
					$(".scheduletab > div > a").animate({top : "+=350px",
														 width:"300px",
														 height:"300px",
														 borderRadius:"150px",
														 fontSize:"300%"}, 200);

					flag=0;
				 	}
					$("#swater").addClass("animated");


				}

				});

		}
			
		});
			$("#d1").click(function(){

				$(".schedtab").hide("slow");
				$(".hd2").hide();
				$(".schedtabb2").hide();
				$(".hd1").fadeIn("slow");
				$(".schedtab").show("slow");
				$(".schedtabb1").fadeIn("slow");
				$("#d1 > a").addClass("disabled");
				$("#d2 > a").removeClass("disabled");


			});

			$("#d2").click(function(){

				$(".schedtab").hide("slow");
				$(".hd1").hide();
				$(".schedtabb1").hide();
				$(".hd2").fadeIn("slow");
				$(".schedtab").show("slow");
				$(".schedtabb2").fadeIn("slow");
				$("#d2 > a").addClass("disabled");
				$("#d1 > a").removeClass("disabled");

			});




			

				
				
			
			

/* ...................schedule triggers ends........................... */


		  });

		jQuery('.scroll').on('click', function(e){		
				e.preventDefault()
		    
		  jQuery('html, body').animate({
		      scrollTop : jQuery(this.hash).offset().top
		    }, 1500);
		});


		</script>

        <script >
         //$('.footer-block-top').addClass('wow bounceInDown');
         //$('.top').addClass('wow bounceInDown');
         //$('.footer-block-left').addClass('wow slideInLeft');
         //$('.left').addClass('wow slideInLeft');
         //$('.footer-block-right').addClass('wow slideInRight');
         //$('.right').addClass('wow slideInRight');
         // $('.main-header').addClass('wow bounceInUp');
         // $('.sub-header').addClass('wow bounceInUp');
         $('.aboutus-desc').addClass('wow bounceInRight');
         $('.about-img').addClass('wow bounceInLeft');
         $('.top-header').addClass('wow slideInDown');
         
         
        </script>

        <script >
          //$('.footer-block-top').attr({'data-wow-duration':'2s', 'data-wow-delay':'0.1s'});
          //$('.top').attr({'data-wow-duration':'2s', 'data-wow-delay':'0.1s'});
          //$('.footer-block-left').attr({'data-wow-duration':'1s', 'data-wow-delay':'0.2s'});
          //$('.left').attr({'data-wow-duration':'1s', 'data-wow-delay':'0.3s'});
          //$('.footer-block-right').attr({'data-wow-duration':'1s', 'data-wow-delay':'0.2s'});
          //$('.right').attr({'data-wow-duration':'1s', 'data-wow-delay':'0.2s'});
          // $('.main-header').attr({'data-wow-duration':'2.5s', 'data-wow-delay':'0.5s'});
          // $('.sub-header').attr({'data-wow-duration':'2.5s', 'data-wow-delay':'0.2s'});
          $('.aboutus-desc').attr({'data-wow-duration':'1s', 'data-wow-delay':'0.1s'});
          $('.about-img').attr({'data-wow-duration':'1s', 'data-wow-delay':'0.1s'});
          $('.top-header').attr({ 'data-wow-delay':'5.7s', 'data-wow-duration':'1s'});
          
        </script>


        <script>
             $( window ).load(function() { 
              new WOW().init();
          })
        </script>