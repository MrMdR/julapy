/*
 *  ClockCircleB2.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 19/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLOCK_CIRCLE_B2_H
#define CLOCK_CIRCLE_B2_H

#include "ofxBox2dCircle.h"

class ClockCircleB2 : public ofxBox2dCircle
{

public :
	
	 ClockCircleB2	( float radius = 0.001, int color = 0xFFFFFF );
	~ClockCircleB2	();
	
	virtual	void set ( float radius, int color );
	
	float		radius;
	int			colorHex;
	ofColor		color;
	ofPoint		lineUpPoint;
	
	int			spinDir;
	float		spinFrc;
};

#endif