class Users::RegistrationsController < Devise::RegistrationsController
def create
    build_resource(sign_up_params)
 
    if resource.save
      if resource.active_for_authentication?
        set_flash_message :notice, :signed_up if is_navigational_format?
        sign_up(resource_name, resource)
        return render :json => {:success => true}
      else
        set_flash_message :notice, :"signed_up_but_#{resource.inactive_message}" if is_navigational_format?
        expire_session_data_after_sign_in!
        return render :json => {:success => true}
      end
    else
      clean_up_passwords resource
      return render :json => {:success => false}
    end
  end
 
  # Signs in a user on sign up. You can overwrite this method in your own
  # RegistrationsController.
  def sign_up(resource_name, resource)
    sign_in(resource_name, resource)
  end
  def sign_up_params
  #devise_parameter_sanitizer.sanitize(:sign_up).push(:name,:nick,:roll,:college)
 # devise_parameter_sanitizer.for(:sign_up) { |u| u.permit(:name, :nick, :roll,:college, :email, :password, :password_confirmation) }

   params.require(:user).permit(:phone,:email, :password, :password_confirmation, :name, :nick,:roll,:college)
  end
end