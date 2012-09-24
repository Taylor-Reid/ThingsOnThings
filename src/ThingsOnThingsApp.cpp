#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
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
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	void insertAfter(Node* afterMe,Node* newLink);
	void reverseList(Node* sentinel);
	void addNewItem();
	void keyDown( KeyEvent event );

private:
	int frameNum_;
	int itemNum_;
	Node* sentinel_;
	Node* privateSent_;
	static const int appHeight_=480;
	static const int appWidth_=640;
	static const int activeBound_=100;
	bool helpMenu_;

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



void ThingsOnThingsApp::setup()
{
	helpMenu_=false;
	frameNum_=0;

	//initializes sentinel and its subsequent starting nodes
	sentinel_= new Node(0,0,0,0,0,Color8u(0,0,0),0);
	insertAfter(sentinel_,new Node(1,450,300,50,50,Color8u(255,0,0),1));
	insertAfter(sentinel_,new Node(0,500,350,100,100,Color8u(0,255,0),2));

	privateSent_= new Node(0,0,0,0,0,Color8u(0,0,0),0);
	insertAfter(privateSent_,new Node(1,215,40,10,0,Color8u(255,0,0),1));
	insertAfter(privateSent_,new Node(0,230,40,20,20,Color8u(0,255,0),2));
	itemNum_=2;
}

void ThingsOnThingsApp::addNewItem(){
	itemNum_++;
	int shapeNum = itemNum_%2;
	float x = randFloat(0.0f,appWidth_-100.0f);
	float y = randFloat(activeBound_,appHeight_-100.0f);
	float dim = 50+50*((itemNum_+1)%2);
	int c1=((itemNum_)%3)*127;
	int c2=((itemNum_+1)%3)*127;
	int c3=((itemNum_+2)%3)*127;
	insertAfter(sentinel_,new Node(shapeNum,x,y,dim,dim,Color8u(c1,c2,c3),itemNum_));
	insertAfter(privateSent_,new Node(shapeNum,200+itemNum_*15,40,dim/5,dim/5,Color8u(c1,c2,c3),itemNum_));
}

void ThingsOnThingsApp::mouseDown( MouseEvent event )
{
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
		insertAfter(sentinel_,object);
		reverseList(sentinel_);
	}
}

void ThingsOnThingsApp::keyDown( KeyEvent event )
{
	//if '?' key is pressed, display help menu
	if( event.getChar() == '?' ) {
		helpMenu_=!helpMenu_;
	}

	if( event.getChar() == ' ' ) {
		reverseList(sentinel_);
	}

	if( event.getChar() == 'a' ) {
		addNewItem();
	}

}

void ThingsOnThingsApp::update()
{
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
	
}

void ThingsOnThingsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );

	Node* temp = sentinel_->next_;
	do{
		gl::color(temp->shade_);
		if(temp->isSquare_){
			gl::drawSolidRect(cinder::Rectf(temp->xPos_,temp->yPos_,temp->xPos_ + temp->width_,temp->yPos_ + temp->height_));
		}else{
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_);
		}
		temp=temp->next_;
	}while(temp != sentinel_);

	gl::color(Color8u(200,200,200));
	gl::drawSolidRect(cinder::Rectf(0,0,640,activeBound_));
	gl::color(Color8u(100,100,100));
	gl::drawSolidRect(cinder::Rectf(150-2,10-2,490+2,activeBound_-10+2));
	gl::color(Color8u(0,0,0));
	gl::drawSolidRect(cinder::Rectf(150,10,640-150,activeBound_-10));

	temp = privateSent_->next_;
	do{
		gl::color(temp->shade_);
		if(temp->isSquare_){
			gl::drawSolidRect(cinder::Rectf(temp->xPos_,temp->yPos_,temp->xPos_ + temp->width_,temp->yPos_ + temp->height_));
		}else{
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_+temp->radius_,temp->yPos_+temp->radius_),temp->radius_);
		}
		temp=temp->next_;
	}while(temp != privateSent_);

	//Displays help menu if it has been activated
	if(helpMenu_){
		gl::color(Color8u(100,100,100));
		gl::drawSolidRect(cinder::Rectf(0,activeBound_,640,appHeight_));
		gl::color(Color8u(200,200,200));
		gl::drawSolidRect(cinder::Rectf(0+2,activeBound_+2,640-2,appHeight_-2));
	}
	
}

CINDER_APP_BASIC( ThingsOnThingsApp, RendererGl )
