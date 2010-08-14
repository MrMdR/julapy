/*
 *  ColorPhysics.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxScreen.h"
#include "ofxBox2d.h"
#include "ColorCircle.h"

class ColorPhysics
{

public:
	
	ColorPhysics();
	~ColorPhysics();

	void setScreen	( ofxScreen screen );
	void setGravity	( float gx, float gy );
	
	void setup	();
	void update	();
	void draw	();
	
	void  createBounds	();
	void  createCircles	();
	float areaToRadius	( float area );
	
	void  createJoints	();
	void  destroyJoints	();
	
	ofxScreen				screen;
	
	ofxBox2d*				box2d;
	b2Body*					ground;
	vector<ColorCircle*>	circles;
	ofPoint					gravity;
	
};