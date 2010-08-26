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
	
	void  createBounds		();
	void  createCircles		();
	void  addCircle			();
	bool  addSingleCircle	();
	bool  removeCircle		();
	float areaToRadius		( float area );
	bool  checkHit			( float x, float y );
	
	void  showCircles	();
	void  hideCircles	();
	
	void  createJoints			();
	void  createJointsForCircle	( ColorCircle* circle );
	void  destroyJoints			();
	void  resetJoints			();
	
	ofxScreen				screen;
	
	ofxBox2d*					box2d;
	b2Body*						ground;
	vector<ColorCircle*>		circles;
	vector<b2DistanceJoint*>	joints;
	ofPoint						gravity;
	
	float						circleRadius;
	
};