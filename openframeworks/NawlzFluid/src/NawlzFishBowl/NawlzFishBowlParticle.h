/*
 *  WanderBoid.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 13/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"

class NawlzFishBowlParticle
{

public:
	
	 NawlzFishBowlParticle ();
	~NawlzFishBowlParticle ();
	
	void		setLoc			( float x, float y );
	void		setVel			( float x, float y );
	void		setBounds		( const ofRectangle& rect );
	void		setImageBounds	( const ofRectangle& rect, unsigned char* pixels, int pixelDepth );
	void		setTexture		( ofTexture* tex );
	
	void		update		();
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
	
	float size;
	float ease;			// Maximum steering force
	float maxspeed;    // Maximum speed

	float wanderTheta;
	float wanderRadius;				// Radius for our "wander circle"
	float wanderDistance;			// Distance for our "wander circle"
	float wanderChange;
	
	float rotation;
	float rotationVel;
	
	vector<int>		colors;
	int				color;
	
	bool			bUseImageForBounds;
	ofRectangle		imageRect;
	unsigned char*	imagePixels;
	int				imagePixelDepth;
	
	ofTexture*	tex;
};