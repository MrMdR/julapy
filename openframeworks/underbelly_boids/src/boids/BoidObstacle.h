/*
 *  BoidObstacle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BOID_OBSTACLE_H
#define BOID_OBSTACLE_H

#include "BoidForce.h"

class BoidObstacle : public BoidForce
{

public :
	
	BoidObstacle	( float x=0, float y=0, float size=0, float reach=0, float magnitude=0 );
	~BoidObstacle	();
	
	void draw		();
	
};

#endif