/*
 *  Ball2D.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BALL_2D_H
#define BALL_2D_H

#include "ofMain.h"

struct b2Ball
{
	double x;
	double y;
	double vx;
	double vy;
	double mass;
	double radius;
};

class Ball2d
{

public :
	
	 Ball2d();
	~Ball2d();
	
	void setGravity			( float x, float y );
	void setVelocityLimit	( float limit );
	void addBall			( double x, double y, double vx, double vy, double mass, double radius );
	void update				();
	
	ofRectangle			walls;
	vector<b2Ball>		balls;
	
private :
	
	void collision2D	( b2Ball &b1, b2Ball &b2, double restitution );
	
	float				gravityX;
	float				gravityY;
	float				velocityLimit;
	
};

#endif