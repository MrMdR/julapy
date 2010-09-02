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
#include "ColorConstants.h"
#include "ColorSound.h"

#ifdef TARGET_OF_IPHONE
#include "ofxALSoundPlayer.h"
#endif

class ColorPhysics : public b2ContactListener
{

public:
	
	ColorPhysics();
	~ColorPhysics();

	void Add	( const b2ContactPoint* point );
	void Remove	( const b2ContactPoint* point );
	
	void setBox2d	( ofxBox2d* box2d );
	void setScreen	( ofxScreen screen );
	void setGravity	( float gx, float gy );
	void setSounds	( ColorSound* sounds );
	
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
	
	int   circlesSize				();
	ofPoint getCirclePointAt		( int i );
	ColorCircle* getCircleAtPoint	( const ofPoint& p );
	ofPoint getCircleColorPointAt	( int i );
	void  setCircleColor			( int i, const ofColor& c );
	void  circleDownAtPoint			( int x, int y, int id );
	void  circleDragAtPoint			( int x, int y, int id );
	void  circleUpAtPoint			( int x, int y, int id );
	float getCollisionAtPoint		( const ofPoint& p );
	
	void  createJoints			();
	void  createJointsForCircle	( ColorCircle* circle );
	void  destroyJoints			();
	void  resetJoints			();
	
	ofxScreen					screen;
	ColorSound*					sounds;
	
	ofxBox2d*					box2d;
	b2Body*						ground;
	vector<ColorCircle*>		circles;
	vector<b2DistanceJoint*>	joints;
	ofPoint						gravity;
	
	float						circleRadius;
};