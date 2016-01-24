class CreateUsersEventsTable < ActiveRecord::Migration
  def self.up
    create_table :users_events, :id => false do |t|
        t.references :user
        t.references :event
    end
    add_index :users_events, [:user_id, :event_id]
    add_index :users_events, :user_id
  end

  def self.down
    drop_table :users_events
  end
end
