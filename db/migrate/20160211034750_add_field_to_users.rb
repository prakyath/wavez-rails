class AddFieldToUsers < ActiveRecord::Migration
  def change
  	add_column :users, :robo, :string
  	add_column :users, :rc, :string
  	add_column :users, :ansys, :string
  	add_column :users, :matlab, :string
  	add_column :users, :webd, :string
  end
end
