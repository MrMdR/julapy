/*
 *  BoidForce.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BOID_FORCE_H
#define BOID_FORCE_H

#include "ofMain.h"

class BoidForce
{
	
public :
	
	 BoidForce	( float x=0, float y=0, float size=0, float reach=0, float magnitude=0 );
	~BoidForce	();
	
	float		x;				// x position of force.
	float		y;				// y position of force.
	ofPoint		point;			// center position of force, combination of x and y.
	float		size;			// physical size (radius) in pixels.
	float		reach;			// the reach this force has from its center position.
	float		magnitude;		// the magnitude / strength of the force.
};

#endif