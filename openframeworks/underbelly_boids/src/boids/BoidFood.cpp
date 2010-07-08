/*
 *  BoidFood.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoidFood.h"

BoidFood :: BoidFood ()
{
	//
}

BoidFood :: ~BoidFood ()
{
	//
}

void BoidFood :: setPosition ( float x, float y )
{
	pos.x = x;
	pos.y = y;
}

void BoidFood :: setRadius ( float r )
{
	radius	= r;
	aura	= r * 3;
}

void BoidFood :: draw ()
{
	ofFill();
	ofSetColor( 100, 100, 100, 128 );
	ofCircle( pos.x, pos.y, radius );
}