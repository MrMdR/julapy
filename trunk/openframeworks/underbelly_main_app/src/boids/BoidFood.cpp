/*
 *  BoidFood.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoidFood.h"

BoidFood :: BoidFood ( float x, float y, float size, float reach, float magnitude )
{
	BoidForce :: init( x, y, size, reach, magnitude );
}

BoidFood :: ~BoidFood ()
{
	//
}

BoidForce BoidFood :: getForceOne ()					//-- first force :: attraction to food source from a far.
{
	return BoidForce( x, y, size, size * 4, 20 );
}

BoidForce BoidFood :: getForceTwo ()					//-- second force :: sharp repulsion from food when contact is made.
{
	return BoidForce( x, y, size, size - 5, -20 );
}

void BoidFood :: draw ()
{
	ofEnableAlphaBlending();
	ofSetColor( 0, 255, 255, 128 );
	ofCircle( x, y, size * 3 );
	ofCircle( x, y, size );
	ofDisableAlphaBlending();
}