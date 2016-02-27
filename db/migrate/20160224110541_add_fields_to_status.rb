class AddFieldsToStatus < ActiveRecord::Migration
  def change
    add_column :statuses, :score, :integer
    add_column :statuses, :user_id, :integer
  end
end
