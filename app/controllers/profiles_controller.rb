class ProfilesController < ApplicationController
puts "my custom controller called yay!!"
def show
  @user=current_user
end	


def edit
end
			
def update 
  @user=current_user
  @user.avatar=user_params[:avatar]
  @user.expo=user_params[:expo]
  @user.save
#   @user.update_attribute(:avatar,user_params)
    respond_to do |format|	
     if  @user.save 
       format.html { redirect_to root_path}
      else
        puts @user.errors.messages
        format.html { render :show }
      end
    end
end


private
def user_params
    params.require(:user).permit(:avatar,:expo)
end

end	