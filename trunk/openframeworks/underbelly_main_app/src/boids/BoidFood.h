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
#include "BoidForce.h"

class BoidFood : public BoidForce
{

public :
	
	 BoidFood	( float x=0, float y=0, float size=0, float reach=0, float magnitude=0 );
	~BoidFood	();
	
	BoidForce getForceOne();
	BoidForce getForceTwo();
	
	void draw	();
	
};

#endif