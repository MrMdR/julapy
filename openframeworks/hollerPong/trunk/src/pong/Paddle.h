/*
 *  Paddle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HOLLER_PONG_PADDLE
#define HOLLER_PONG_PADDLE

#include "ofMain.h"

class Paddle
{
	
public :
	
	Paddle();
	
	void setColor		( int color );
	void setSide		( int side );
	void setPosition	( float p );
	void update			();
	void draw			();
	
	ofRectangle	rect;
	int			color;
	float		pos;
	int			side;
	
};

#endif