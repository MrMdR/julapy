/*
 *  ClockCircle.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLOCK_CIRCLE_H
#define CLOCK_CIRCLE_H

#include "ofxBox2dCircle.h"

class ClockCircle : public ofxBox2dCircle
{

public :
	
	 ClockCircle		( float radius = 0.001, int color = 0xFFFFFF );
	~ClockCircle		();

	void update			();
	void draw			();
	
	void set			( float radius, int color );
	void enableGravity	( bool b );
	
	//--
	
	float		radius;
	int			colorHex;
	ofColor		color;
	ofPoint		lineUpPoint;
	
	int			spinDir;
	float		spinFrc;
	
	bool		active;
	
	ofPoint		eye;
};

#endif