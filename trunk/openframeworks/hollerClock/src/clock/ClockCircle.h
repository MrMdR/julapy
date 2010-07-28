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

#include "ofxBox2d.h"

class ClockCircle : public ofxBox2dCircle
{

public :
	
	 ClockCircle		( float radius = 0.001, int color = 0xFFFFFF );
	~ClockCircle		();

	void setSize		( ofRectangle &size );
	void setSize		( int w, int h );
	
	void update			();
	void draw			();
	
	void createJoint	();
	void destroyJoint	();
	bool hasJoint		();
	
	int			screenWidth;
	int			screenHeight;
	
	float		radius;
	int			colorHex;
	ofColor		colorTrgt;
	ofColor		colorCurr;
	
	ofPoint		lineUpPoint;
	
	int			spinDir;
	float		spinFrc;
	
	bool		active;
	
	ofPoint		eye;
	
	b2Body*				point;
	b2DistanceJoint*	joint;
};

#endif