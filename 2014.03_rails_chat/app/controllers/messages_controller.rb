class MessagesController < ApplicationController
  def index
    @messages = Message.all
  end
  
  def create
    @post = Message.new(params.require(:message).permit(:nickname, :message))
    @post.save
    
    index
    render action: 'index'
  end
  
  def destroy
    Message.find(params[:id]).destroy
    
    
    index
    redirect_to messages_path
  end
end
