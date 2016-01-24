class AddNickToUser < ActiveRecord::Migration
  def change
  	 add_column :users, :phone,:integer, :limit => 8
  	 add_column :users, :roll, :text
  	 add_column :users, :college, :text
  	 add_column :users, :event_ids, :integer


  end
end
