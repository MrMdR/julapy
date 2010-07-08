/*
 *  BoidFood.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BOID_FOOD_H
#define BOID_FOOD_H

#include "ofMain.h"
#include "ofxVectorMath.h"

class BoidFood
{

public :
	
	 BoidFood();
	~BoidFood();
	
	void setPosition	( float x, float y );
	void setRadius		( float r );
	
	void draw			();

	ofxVec2f			pos;
	float				radius;
	float				aura;
	
};

#endif