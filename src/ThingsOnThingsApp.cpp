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

int frameNum=0;

class ThingsOnThingsApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void ThingsOnThingsApp::prepareSettings(Settings* settings){
(*settings).setWindowSize(640,480);
(*settings).setResizable(false);
}

void ThingsOnThingsApp::setup()
{
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
	gl::color(Color8u(200,200,200));
	gl::drawSolidRect(cinder::Rectf(0,0,800,100));
	 
	frameNum++;
}

CINDER_APP_BASIC( ThingsOnThingsApp, RendererGl )
