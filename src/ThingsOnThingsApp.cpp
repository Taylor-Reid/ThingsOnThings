/**
* @file ThingsOnThingsApp.cpp
* CSE 274 - Fall 2012
* My solution for HW02.
*
* @author Taylor Reid
* @date 2012-09-24
*
* @note This file is (c) 2012. It is licensed under the
* CC BY 3.0 license (http://creativecommons.org/licenses/by/3.0/),
* which means you are free to use, share, and remix it as long as you
* give attribution. Commercial uses are allowed.
*
* @note This project satisfies main goals : (A) linked list structure, (B) help message, (C) reordering of items, (D) movement
*											(E) reversing with one button, 
*							stretch goals : (J) game : incomplete.
*				Game goals to be finished : Randomizes the desired order, if player sets shapes to that order, player receives
											how ever many points are still on the clock. The game will the go on to the next round,
											adding one more shape, re-randomizing the order, and resetting the score clock. 
											Game ends if score clock goes to 0, or player completes round 15. 
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/Surface.h"
#include "cinder/Cinder.h"
#include "cinder/Font.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include "Node.h"

;
using namespace ci;
using namespace ci::app;
using namespace std;



class ThingsOnThingsApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void keyDown( KeyEvent event );
	void update();
	void draw();
	void prepareSettings(Settings* settings);

	/*
		Sends puts a newly created node after the indicated node.
		Does not work for nodes already in the list. Use sendToFront for that purpose.
	*/
	void insertAfter(Node* afterMe,Node* newLink);

	/*
		Take's a node from in the list, and sends it to the front of the list. Note: this means it will draw in the back
		of the image, if wanting to send to front of image you must reverse the list, call this function, then reverse again.
	*/
	void sendToFront(Node* sentinel,Node* toMove);

	/*
		Reverses the order of the list.
	*/
	void reverseList(Node* sentinel);
	void addNewItem();
	

private:
	int frameNum_;
	int itemNum_;
	Node* sentinel_;
	Node* privateSent_;
	Node* relativeSent_;
	static const int appHeight_=480;
	static const int appWidth_=640;
	static const int activeBound_=100;
	bool helpMenu_;
	bool justStarted_;
	Font font_;
	gl::Texture texture_;
	gl::Texture texture2_;
	int score_;
	int scoreTotal_;
	int round_;
	int selected_;
	float timeEst_;
};

void ThingsOnThingsApp::prepareSettings(Settings* settings){
(*settings).setWindowSize(appWidth_,appHeight_);
(*settings).setResizable(false);
}

void ThingsOnThingsApp::insertAfter(Node* afterMe,Node* newLink){
	newLink->next_=afterMe->next_;
	afterMe->next_=newLink;
}

void ThingsOnThingsApp::reverseList(Node* sentinel){
	Node* prev = sentinel;
	Node* cur = prev->next_;
	Node* temp = cur->next_;
	do{
		cur->next_=prev;
		prev=cur;
		cur=temp;
		temp=cur->next_;
	}while(prev != sentinel);
}

void ThingsOnThingsApp::sendToFront(Node* sentinel,Node* toMove){
	Node* temp = sentinel->next_;
	Node* prev = sentinel;
	bool found = false;
	do{
		if(temp==toMove){
			found=true;
		}else{
			prev=prev->next_;
			temp = prev->next_;
		}
	}while(!found);
	prev->next_=temp->next_;
	temp->next_=sentinel->next_;
	sentinel->next_=temp;
}



void ThingsOnThingsApp::setup()
{
	helpMenu_=true;
	justStarted_=true;
	frameNum_=0;
	score_=0;
	round_=1;
	scoreTotal_=0;
	selected_=0;
	timeEst_=0.0f;

	font_=Font("Cambria",20);

	//initializes sentinel and its subsequent starting nodes
	sentinel_= new Node(0,0,0,0,0,Color8u(0,0,0),0);
	insertAfter(sentinel_,new Node(1,450,300,50,50,Color8u(255,0,0),1));
	insertAfter(sentinel_,new Node(0,500,350,100,100,Color8u(0,255,0),2));

	privateSent_= new Node(0,0,0,0,0,Color8u(0,0,0),0);
	insertAfter(privateSent_,new Node(1,215,25,10,0,Color8u(255,0,0),1));
	insertAfter(privateSent_,new Node(0,230,25,20,20,Color8u(0,255,0),2));

	relativeSent_= new Node(0,0,0,0,0,Color8u(0,0,0),0);
	insertAfter(relativeSent_,new Node(1,215,55,10,0,Color8u(255,0,0),1));
	insertAfter(relativeSent_,new Node(0,230,55,20,20,Color8u(0,255,0),2));

	itemNum_=2;
}

void ThingsOnThingsApp::addNewItem(){
	if (itemNum_ < 14){
		itemNum_++;
		int shapeNum = itemNum_%2;
		float x = randFloat(0.0f,appWidth_-100.0f);
		float y = randFloat(activeBound_,appHeight_-100.0f);
		float dim = 50+50*((itemNum_+1)%2);
		int c1=((itemNum_)%3)*127;
		int c2=((itemNum_+1)%3)*127;
		int c3=((itemNum_+2)%3)*127;
		insertAfter(sentinel_,new Node(shapeNum,x,y,dim,dim,Color8u(c1,c2,c3),itemNum_));
		insertAfter(privateSent_,new Node(shapeNum,200+itemNum_*15,25,dim/5,dim/5,Color8u(c1,c2,c3),itemNum_));
		insertAfter(relativeSent_,new Node(shapeNum,200+itemNum_*15,55,dim/5,dim/5,Color8u(c1,c2,c3),itemNum_));
	}
}

void ThingsOnThingsApp::mouseDown( MouseEvent event )
{	/*
	int x= event.getX();
	int y= event.getY();
	int tx;
	int ty;
	int centerx;
	int centery;
	int dx;
	int dy;

	Node* temp=sentinel_->next_;
	Node* object = sentinel_; 
	//starts at front of list (back of image) , saves a temp of any shape that is under the mouse pointer
	// overwrites if a new shape is under mouse pointer. Results in the top most (the object seen under the mouse pointer) being saved.
	do{
		tx = temp->xPos_;
		ty = temp->yPos_;
		if(temp->isSquare_){
			if(x>=tx && x<=(tx+temp->width_) && y>=ty && y<=(ty+temp->height_)){
				object=temp;
			}
		}else{
			centerx=tx+temp->radius_;
			centery=ty+temp->radius_;
			dx = x-centerx;
			dy = y-centery;
			if((dx*dx+dy*dy)<=temp->radius_){
				object=temp;
			}
		}

		temp=temp->next_;
	}while(temp != sentinel_);
	//if object is not null, it sends it to front of image (back of list)
	if (object != sentinel_){
		reverseList(sentinel_);
		sendToFront(object);
		reverseList(sentinel_);
	}
	*/
}

void ThingsOnThingsApp::keyDown( KeyEvent event )
{
	//if '?' key is pressed, display help menu
	if( event.getChar() == '?' ) {
		helpMenu_=!helpMenu_;
	}

	if( event.getChar() == ' ' ) {
		reverseList(sentinel_);
		reverseList(relativeSent_);
	}

	if( event.getChar() == 'a' ) {
		addNewItem();
	}
	if( event.getChar() == 'v' ) {//changes selected object
		if(selected_<itemNum_-1){
			selected_++;
		}
	}
	if( event.getChar() == 'b' ) {//sends selected object to front.
		Node* temp = sentinel_->next_;
		Node* relativeTemp = relativeSent_->next_;
		for(int i = 0; i<selected_; i++){
			temp=temp->next_;
			relativeTemp=relativeTemp->next_;
		}
		reverseList(sentinel_);
		sendToFront(sentinel_,temp);
		reverseList(sentinel_);

		reverseList(relativeSent_);
		sendToFront(relativeSent_,relativeTemp);
		reverseList(relativeSent_);
		
		selected_=itemNum_-1;

	}
	if( event.getChar() == 'n' ) {//changes selected object
		if(selected_>0){
			selected_--;
		}
	}
	/*
	if( event.getChar() == 'c' ) {
		Vec2i xy =getMousePos();
		int x = xy.x;
		int y = xy.y;
		int tx;
		int ty;
		int centerx;
		int centery;
		int dx;
		int dy;
	
		Node* temp=sentinel_->next_;
		Node* object = sentinel_; 
		//starts at front of list (back of image) , saves a temp of any shape that is under the mouse pointer
		// overwrites if a new shape is under mouse pointer. Results in the top most (the object seen under the mouse pointer) being saved.
		do{
			tx = temp->xPos_;
			ty = temp->yPos_;
			if(temp->isSquare_){
				if(x>=tx && x<=(tx+temp->width_) && y>=ty && y<=(ty+temp->height_)){
					object=temp;
				}
			}else{
				centerx=tx+temp->radius_;
				centery=ty+temp->radius_;
				dx = x-centerx;
				dy = y-centery;
				if((dx*dx+dy*dy)<=temp->radius_){
					object=temp;
				}
			}
				
			temp=temp->next_;
		}while(temp != sentinel_);
		//if object is not null, it sends it to front of image (back of list)
		if (object != sentinel_){
			reverseList(sentinel_);
			insertAfter(sentinel_,object);
			reverseList(sentinel_);
		}
	}
	*/

}

void ThingsOnThingsApp::update()
{
	//get start time
	if(justStarted_){
		justStarted_=false;
		timeEst_=0.0f;
	}
	//update basic variables
	frameNum_++;
	score_=int(60-timeEst_);
	timeEst_=timeEst_+0.1f;
	

	//updates node positions, sizes , checks to make sure not out of bounds
	Node* temp = sentinel_->next_;
	do{
		//modifies position
		temp->xPos_ += temp->vx_;
		temp->yPos_ += temp->vy_;
		//checks in case of rectangle:
		if(temp->isSquare_){
			if((temp->xPos_ + temp->width_)>= appWidth_ || temp->xPos_ <= 0 ){
				temp->vx_= -temp->vx_;
				temp->xPos_+= temp->vx_;
			}
			if((temp->yPos_ + temp->height_)>= appHeight_ || temp->yPos_ <= activeBound_ ){
				temp->vy_= -temp->vy_;
				temp->yPos_ += temp->vy_;
			}
		}else{//in case of circle
			if((temp->xPos_ + 2*temp->radius_)>= appWidth_ || temp->xPos_ <= 0 ){
				temp->vx_= -temp->vx_;
				temp->xPos_+= temp->vx_;
			}
			if((temp->yPos_ + 2*temp->radius_)>= appHeight_ || temp->yPos_ <= activeBound_ ){
				temp->vy_= -temp->vy_;
				temp->yPos_ += temp->vy_;
			}
		}
		//modifies size
		//in case of rectangle:
		if(temp->isSquare_){
			temp->width_+=temp->dr_;
			temp->height_+=temp->dr_;
			if(temp->width_<50 || temp->width_>150){
				temp->dr_=-temp->dr_;
			}
		}else{//in case of circle
			temp->radius_+=temp->dr_;
			if(temp->radius_<25 || temp->radius_>75){
				temp->dr_=-temp->dr_;
			}
		}
		temp=temp->next_;
	}while(temp != sentinel_);

	//updates relativeSent list to fix position:
	reverseList(relativeSent_);
	temp = relativeSent_->next_;
	int count = 1;
	do{
		temp->xPos_=200+count*15;
		temp=temp->next_;
		count++;
	}while(temp != relativeSent_);
	reverseList(relativeSent_);

}

void ThingsOnThingsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );

	int count = 0; 

	Node* temp = sentinel_->next_;
	do{
		if(temp->isSquare_){
			//draws white border around selected object
			if(count == selected_){
				gl::color(Color8u(255,255,255));
				gl::drawSolidRect(cinder::Rectf(temp->xPos_-3,temp->yPos_-3,temp->xPos_ +3+ temp->width_,temp->yPos_ +3+ temp->height_));
			}else{//draws black border on all others
				gl::color(Color8u(0,0,0));
				gl::drawSolidRect(cinder::Rectf(temp->xPos_-3,temp->yPos_-3,temp->xPos_ +3+ temp->width_,temp->yPos_ +3+ temp->height_));
			}
			gl::color(temp->shade_);
			gl::drawSolidRect(cinder::Rectf(temp->xPos_,temp->yPos_,temp->xPos_ + temp->width_,temp->yPos_ + temp->height_));
		}else{//same for circles
			if(count == selected_){
				gl::color(Color8u(255,255,255));
				gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_+3);
			}else{
				gl::color(Color8u(0,0,0));
				gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_+3);
			}
			gl::color(temp->shade_);
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_);
			
		}
		temp=temp->next_;
		count++;
	}while(temp != sentinel_);

	gl::color(Color8u(200,200,200));
	gl::drawSolidRect(cinder::Rectf(0,0,640,activeBound_));

	string txt2 = " Score:                                                                                                                                                       Round: \n\n";
	txt2 = txt2 + " Score Placeholder                                                                                                             Round placeholder \n\n";
	TextBox box2 = TextBox().alignment( TextBox::CENTER ).font(font_).size(640,100).text( txt2 );
	box2.setColor( Color8u( 0,0,0) );
	box2.setBackgroundColor( Color8u( 255,255,255 ) );
	texture2_ = gl::Texture( box2.render() );
	gl::draw(texture2_);
	
	gl::color(Color8u(100,100,100));
	gl::drawSolidRect(cinder::Rectf(150-2,10-2,490+2,activeBound_-10+2));
	gl::color(Color8u(0,0,0));
	gl::drawSolidRect(cinder::Rectf(150,10,640-150,activeBound_-10));
	


	//draws small non moving objects in upper window
	temp = privateSent_->next_;
	do{
		if(temp->isSquare_){
			gl::color(Color8u(0,0,0));
			gl::drawSolidRect(cinder::Rectf(temp->xPos_-1,temp->yPos_-1,temp->xPos_ +1+ temp->width_,temp->yPos_ +1+ temp->height_));
			gl::color(temp->shade_);
			gl::drawSolidRect(cinder::Rectf(temp->xPos_,temp->yPos_,temp->xPos_ + temp->width_,temp->yPos_ + temp->height_));
			
		}else{
			gl::color(Color8u(0,0,0));
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_+1);
			gl::color(temp->shade_);
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_);
		}
		temp=temp->next_;
	}while(temp != privateSent_);

	//draw the ones that represent the big objects positions
	temp = relativeSent_->next_;
	do{
		if(temp->isSquare_){
			gl::color(Color8u(0,0,0));
			gl::drawSolidRect(cinder::Rectf(temp->xPos_-1,temp->yPos_-1,temp->xPos_ +1+ temp->width_,temp->yPos_ +1+ temp->height_));
			gl::color(temp->shade_);
			gl::drawSolidRect(cinder::Rectf(temp->xPos_,temp->yPos_,temp->xPos_ + temp->width_,temp->yPos_ + temp->height_));
		}else{
			gl::color(Color8u(0,0,0));
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_+1);
			gl::color(temp->shade_);
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_);
		}
		temp=temp->next_;
	}while(temp != relativeSent_);

	//Displays help menu if it has been activated
	if(helpMenu_){
		gl::color(Color8u(100,100,100));
		gl::drawSolidRect(cinder::Rectf(0,activeBound_,640,appHeight_));
		gl::color(Color8u(200,200,200));
		gl::drawSolidRect(cinder::Rectf(0+2,activeBound_+2,640-2,appHeight_-2));
		
		string txt = "The Things On Things Game!\n\n Controls: \n Press '?' to open or close this menu. \n Press spacebar to reverse the order of the shapes. \n Press 'a' to add another shape (for testing purposes only,\n incase you don't want to play the game).\n Press 'v' & 'n' to toggle through the objects.\n Press 'b' to send the selected object (outlined in white) to the front. \n\n Goals:\n Get the moving shapes to match the order of the shapes in the small window. \n The less time you take, the more points you get! \n";
		TextBox box = TextBox().alignment( TextBox::CENTER ).font(font_).size(640,480).text( txt );
		box.setColor( Color8u( 0,0,0) );
		box.setBackgroundColor( Color8u( 255,255,255 ) );
		texture_ = gl::Texture( box.render() );
		gl::draw(texture_);
	}
	
}

CINDER_APP_BASIC( ThingsOnThingsApp, RendererGl )
