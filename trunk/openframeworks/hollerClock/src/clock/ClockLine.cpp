/*
 *  ClockLine.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 3/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockLine.h"

ClockLine :: ClockLine ()
{
	tex		= NULL;
	angle	= 0;
	scale	= ofRandom( 0.8, 1.0 );
}

ClockLine :: ~ClockLine ()
{

}

void ClockLine :: setTexture ( ofTexture* tex )
{
	this->tex = tex;
}

void ClockLine :: draw ()
{
	glPushMatrix();
	glTranslatef( 0, -tex->getHeight() * 0.5, 0 );
	glScalef( scale, scale, 0 );
	glRotatef( angle, 0, 0, 1 );
	tex->draw( 0, 0 );
	glPopMatrix();
}