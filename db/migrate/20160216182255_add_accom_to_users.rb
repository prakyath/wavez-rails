class AddAccomToUsers < ActiveRecord::Migration
  def change
    add_column :users, :accom, :string
  end
end
