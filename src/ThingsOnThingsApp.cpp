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

private:
	int frameNum_;
	Node* sentinel_;
	Node* privateSent_;

};

void ThingsOnThingsApp::prepareSettings(Settings* settings){
(*settings).setWindowSize(640,480);
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
	frameNum_=0;
	sentinel_= new Node(0,0,0,0,0,Color8u(0,0,0),0);

	insertAfter(sentinel_,new Node(1,450,300,40,40,Color8u(255,0,0),1));
	insertAfter(sentinel_,new Node(0,500,350,40,40,Color8u(255,0,0),2));

	privateSent_= new Node(0,0,0,0,0,Color8u(0,0,0),0);
	insertAfter(privateSent_,new Node(1,450,300,40,40,Color8u(255,0,0),1));
	insertAfter(privateSent_,new Node(0,500,350,40,40,Color8u(255,0,0),2));
}

void ThingsOnThingsApp::mouseDown( MouseEvent event )
{
}

void ThingsOnThingsApp::update()
{
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
			gl::drawSolidCircle(cinder::Vec2f(temp->xPos_,temp->yPos_),temp->radius_);
		}
		temp=temp->next_;
	}while(temp != sentinel_);

	gl::color(Color8u(200,200,200));
	gl::drawSolidRect(cinder::Rectf(0,0,800,100));
	
}

CINDER_APP_BASIC( ThingsOnThingsApp, RendererGl )
