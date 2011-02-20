/*
 *  NawlzSandParticle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"

class NawlzSandParticle
{
	
public:
	
	 NawlzSandParticle();
	~NawlzSandParticle();
	
	void		setLoc			( float x, float y );
	void		setVel			( float x, float y );
	void		setBounds		( const ofRectangle& rect );
	void		setImageBounds	( const ofRectangle& rect, unsigned char* pixels );
	void		setTexture		( ofTexture* tex );
	
	void		update		( float forceScale = 1.0 );
	void		draw		();
	void		drawDebug	();
	
	void		wander				();
	bool		constrainToBorders	( const ofxVec2f& target );
	bool		constrainToImage	( const ofxVec2f& target );
	ofxVec2f	steer				( const ofxVec2f& target );
	
	ofxVec2f loc;
	ofxVec2f vel;
	ofxVec2f acc;
	
	ofRectangle	bounds;
	ofPoint		boundsCenter;
	
	ofxVec2f circle;
	ofxVec2f circleTarget;
	ofxVec2f circleOffSet;
	
	float sizeRadius;
	float ease;			// Maximum steering force
	float maxspeed;    // Maximum speed
	
	float wanderTheta;
	float wanderRadius;				// Radius for our "wander circle"
	float wanderDistance;			// Distance for our "wander circle"
	float wanderChange;
	
	bool			bUseImageForBounds;
	ofRectangle		imageRect;
	unsigned char*	imagePixels;
	
	ofTexture*	tex;
	
	float friction;
};