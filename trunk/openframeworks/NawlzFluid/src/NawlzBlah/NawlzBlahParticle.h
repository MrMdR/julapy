/*
 *  NawlzBlahParticle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"

class NawlzBlahParticle
{
	
public:
	
	 NawlzBlahParticle ();
	~NawlzBlahParticle ();
	
	void		setLoc			( float x, float y );
	void		setVel			( float x, float y );
	void		setBounds		( const ofRectangle& rect );
	void		setImageBounds	( const ofRectangle& rect, unsigned char* pixels );
	void		setTexture		( ofTexture* tex );
	
	bool		isAlive		();
	
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
	
	ofxVec2f circle;
	ofxVec2f circleTarget;
	ofxVec2f circleOffSet;
	
	float sizeRadius;
	float maxspeed;    // Maximum speed
	
	float wanderTheta;
	float wanderRadius;				// Radius for our "wander circle"
	float wanderDistance;			// Distance for our "wander circle"
	float wanderChange;
	float wanderEase;
	
	bool			bUseImageForBounds;
	ofRectangle		imageRect;
	unsigned char*	imagePixels;
	
	ofTexture*	tex;
	
	int			lifeCount;
	int			lifeLimit;
	float		lifePercent;
	float		lifeFade;
	float		lifeAlpha;
	
	float		rotation;
	float		rotationVel;
	
	float		scale;
	float		scaleMin;
	
	float		friction;
};