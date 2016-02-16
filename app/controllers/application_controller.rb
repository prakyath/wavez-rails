class ApplicationController < ActionController::Base
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
 protect_from_forgery with: :exception















 def method_missing(m, *args, &block)
  Rails.logger.error(m)
  redirect_to :controller=>"errors", :action=>"error_404"
  # or render/redirect_to somewhere else
end





end

unless Rails.application.config.consider_all_requests_local
  rescue_from Exception, :with => :method_missing
  rescue_from ActiveRecord::RecordNotFound, :with => :method_missing
  rescue_from AbstractController::ActionNotFound, :with => :method_missing
  rescue_from ActionController::RoutingError, :with => :method_missing
  rescue_from ActionController::UnknownController, :with => :method_missing
  rescue_from ActionController::UnknownAction, :with => :method_missing
end
















