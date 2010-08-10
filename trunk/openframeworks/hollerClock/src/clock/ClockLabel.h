/*
 *  ClockLabel.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 10/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class ClockLabel : public ofxBox2dRect
{

public:
	
	 ClockLabel ();
	~ClockLabel ();
	
	void setSize		( ofRectangle &size );
	void setSize		( int w, int h );
	void setTexture		( ofTexture* tex );
	void setForceScale	( float scale );
	
	void init			();
	void setup			( b2World* b2dworld, float x, float y, float w, float h, bool isFixed=false );
	void createJoint1	( b2Body* body, float x, float y, float w, float h );
	void createJoint2	( b2Body* body, float x, float y, float w, float h );
	
	void draw			();
	
	int			screenWidth;
	int			screenHeight;
	
	float		forceScale;
	
	ofTexture*	tex;
	
	b2Body*				point1;
	b2DistanceJoint*	joint1;

	b2Body*				point2;
	b2DistanceJoint*	joint2;
};