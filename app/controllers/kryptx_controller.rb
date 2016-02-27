class KryptxController < ApplicationController  

def index
@user=current_user
end

def show
@user=current_user

if @user.status.nil?
 Status.create(score: 1,user_id: @user.id)
end 
render :template => 'kryptx/show' 
end

def update
@user=current_user

if @user.status.nil?
 Status.create(score: 1,user_id: @user.id)
end 

@yo=@user.status
result=params['enter_text'].strip.downcase

case @yo.score

when 1
  if result=='iim ahmedabad'
  flash[:success] = "Congratulation,now try the next one"	
  @yo.score=@yo.score+1
  @yo.save
  end   
when 2
  if result=='53'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 3
  if result=='leo tolstoy'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 4
  if result=='tamil'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 5
  if result=='american academy of arts and sciences'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 6
  if result=='international monetary fund'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 7
  if result=='462001'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 8
  if result=='iit delhi'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 9
  if result=='university of chicago'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 10
  if result=='fault lines'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 11
  if result=='my days with nehru'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 12
  if result=='2/2/1963'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 13
  if result=='wolf of wall street'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end   
when 14
  if result=='fischer black prize'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end 
when 15
  if result=='we are neither hawks nor doves we are actually owls'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end
when 16
  if result=='american finance association'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end
when 17
  if result=='inside job'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end
when 18
  if result=='g30'
  flash[:success] = "Congratulation,now try the next one"  
  @yo.score=@yo.score+1
  @yo.save
  end
when 19
  if result=='raghuram rajan'
  flash[:success] = "Congratulation,You have completed all the questions!!"  
  @yo.score=@yo.score+1
  @yo.save
  end            
else
  puts "what can be done da?" 
end

render :template => 'kryptx/show' 


end





end
