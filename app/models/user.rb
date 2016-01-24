class User < ActiveRecord::Base
  # Include default devise modules. Others available are:
  # :confirmable, :lockable, :timeoutable and :omniauthable
  devise :database_authenticatable, :registerable,
         :recoverable, :rememberable, :trackable, :validatable

  validates :password, confirmation: true
  validates :password_confirmation, presence: true 
  validates :name, presence: true
  validates :nick, presence: true, uniqueness: true
  validates :email, presence: true, uniqueness: true
  validates :college, presence: true
  validates :roll, presence: true, uniqueness: true     
end

