/*
 *  NawlzParticle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzParticle.h"

NawlzParticle :: NawlzParticle( ofTexture* texIn, const Vec2f& pos, const Vec2f& vel )
{
	tex		= NULL;
	width	= 4;
	height	= 4;
	alpha	= 0.85;
	mass	= ofRandom( 0.1, 1.0 );
	
	if( texIn )
	{
		tex		= texIn;
		
		width	= tex->getWidth();
		height	= tex->getHeight();
	}
	
	this->pos = pos;
	this->vel = vel;
}

NawlzParticle :: ~NawlzParticle()
{
	tex	= NULL;
}

void NawlzParticle :: draw ()
{
	ofFill();
	ofSetColor( 255, 255, 255, 255 * alpha );
	
	if( tex )
	{
		tex->draw( pos.x - width * 0.5, pos.y - height * 0.5 );
	}
	else
	{
		ofRect( pos.x - width * 0.5, pos.y - height * 0.5, width, height );
	}
}
