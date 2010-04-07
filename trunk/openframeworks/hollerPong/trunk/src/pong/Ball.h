/*
 *  Ball.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HOLLER_PONG_BALL
#define HOLLER_PONG_BALL

#include "ofMain.h"
#include "ofxVectorMath.h"

class Ball
{

public :
	
	Ball();
	
	void setPositon		( float x, float y );
	void setVelocity	( float x, float y );
	
	void update		();
	void draw		();
	
	ofxVec2f	loc;
	ofxVec2f	vel;
	float		radius;
	int			color;
	
};

#endif