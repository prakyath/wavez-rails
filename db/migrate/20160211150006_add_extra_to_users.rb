class AddExtraToUsers < ActiveRecord::Migration
  def change
  	add_column :users, :awc, :string
  	add_column :users, :roboe, :string
  end
end
