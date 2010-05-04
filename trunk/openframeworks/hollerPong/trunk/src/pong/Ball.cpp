/*
 *  Ball.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Ball.h"

Ball :: Ball()
{
	radius		= PONG_BALL_RADIUS;
	rotation	= 0;
	rotationDir	= 1;
	color		= 0xFFFFFF;
	
	setPositon( 0, 0 );
	setVelocity( 1, 1 );
}

void Ball :: setPositon ( float x, float y )
{
	loc.x = x;
	loc.y = y;
}

void Ball :: setVelocity ( float x, float y )
{
	vel.x = x;
	vel.y = y;
}

void Ball :: toggleRotationDir ()
{
	rotationDir *= -1;
}

void Ball :: update ()
{
	loc			+= vel;
	rotation	+= rotationDir;
}

void Ball :: draw ()
{
	ofSetColor( 0xFFFFFF );
	
	ofFill();
	ofEllipse( loc.x, loc.y, radius * 2, radius * 2 );
	
	ofNoFill();
	ofEllipse( loc.x, loc.y, radius * 2, radius * 2 );
}