class AddNickToUsers < ActiveRecord::Migration
  	 def change
  	 add_column :users, :name, :text
  	 add_column :users, :nick, :text


  end
end
