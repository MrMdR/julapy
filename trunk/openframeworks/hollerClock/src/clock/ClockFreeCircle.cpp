/*
 *  ClockFreeCircle.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 10/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockFreeCircle.h"

ClockFreeCircle :: ClockFreeCircle ()
{
	//
}

ClockFreeCircle :: ~ClockFreeCircle ()
{
	//
}

void ClockFreeCircle :: update ()
{
	body->WakeUp();
}

void ClockFreeCircle :: draw ()
{
	ofSetColor( 0, 0, 0, 30 );
	
	glPushMatrix();
	glTranslatef( getPosition().x, getPosition().y, 0 );
	
	ofCircle( 0, 0, getRadius() - 1 );
	
	glPopMatrix();
}