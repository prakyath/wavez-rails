class AddFieldToCategorization < ActiveRecord::Migration
  def change
  	 add_column :categorizations, :user_id, :integer
  	 add_column :categorizations, :event_id, :integer
  end
end
