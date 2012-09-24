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



Node::Node(int shapeNum, float x, float y, float dim1, float dim2,Color8u col,int idNum){
	next_=this;
	hasSub_=false;

	shade_=col;

	xPos_=x;
	yPos_=y;

	vx_=randFloat(0.3f,1.0f);
	vy_=randFloat(0.3f,1.0f);

	dr_=randFloat(0.00f,0.3f);

	id_=idNum;

	if(shapeNum==0){//Rectangle
		width_=dim1;
		height_=dim2;
		isSquare_=true;
		isCircle_=false;
	}else if(shapeNum==1){//Circle
		radius_=dim1;
		isSquare_=false;
		isCircle_=true;
	}else{//forces rectangle if not given 0 or 1
		shapeNum=0;
		width_=dim1;
		height_=dim2;
		isSquare_=true;
		isCircle_=false;
	}
}