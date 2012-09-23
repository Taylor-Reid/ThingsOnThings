/*

Notes & info

*/

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



Node::Node(int shapeNum, int x, int y, int dim1, int dim2,Color8u col){
	next_=this;
	hasSub_=false;

	shade_=col;

	xPos_=x;
	yPos_=y;
	if(shapeNum=0){//Rectangle
		width_=dim1;
		height_=dim2;
	}else if(shapeNum=1){//Circle
		radius_=dim1;
	}else{

	}
}