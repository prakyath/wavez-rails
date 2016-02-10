class AddExpoToUsers < ActiveRecord::Migration
  def change
    add_column :users, :expo, :string
  end
end
