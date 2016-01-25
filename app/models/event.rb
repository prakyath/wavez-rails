class Event < ActiveRecord::Base
	has_many :users, through: :categorizations
	has_many :categorizations
	
end
