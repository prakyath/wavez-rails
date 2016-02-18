class User < ActiveRecord::Base
  has_many :events, through: :categorizations
  has_many :categorizations
  # Include default devise modules. Others available are:
  # :confirmable, :lockable, :timeoutable and :omniauthable
  devise :database_authenticatable, :registerable,
         :recoverable, :rememberable, :trackable, :validatable
  
  mount_uploader :avatar, AvatarUploader
  mount_uploader :avatar1, AvatarUploader
  validates :password, confirmation: true
#  validates :password_confirmation, presence: true 
  validates :name, presence: true
  validates :nick, presence: true
  validates :phone, presence: true
  validates :email, presence: true, uniqueness: true
  validates :college, presence: true
  validates :roll, presence: true 
end

