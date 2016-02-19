class AddSomeToUsers < ActiveRecord::Migration
  def change
    add_column :users, :avatar1, :string
    add_column :users, :freestyle, :string
    add_column :users, :research, :string
    add_column :users, :game, :string
  end
end
