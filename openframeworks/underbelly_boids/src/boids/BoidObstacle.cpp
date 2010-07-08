/*
 *  BoidObstacle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoidObstacle.h"

BoidObstacle :: BoidObstacle ( float x, float y, float size, float reach, float magnitude )
{
	BoidForce :: init( x, y, size, reach, magnitude );
}

BoidObstacle :: ~BoidObstacle()
{

}

void BoidObstacle :: draw ()
{
	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 0, 128 );
	ofCircle( x, y, size );
	ofDisableAlphaBlending();
}