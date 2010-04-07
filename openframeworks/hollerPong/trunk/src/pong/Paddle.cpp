/*
 *  Paddle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Paddle.h"

Paddle :: Paddle()
{
	rect.width	= 40;
	rect.height	= 200;
	
	setColor( 0x000000 );
	setSide( 0 );
	setPosition( 0.5 );
}

void Paddle :: setColor	( int c )
{
	color = c;
}

void Paddle :: setSide	( int s )
{
	side = s;
}

void Paddle :: setPosition ( float p )
{
	pos	= MIN( 1, MAX( p, 0 ) );
}

void Paddle :: update ()
{
	//
}

void Paddle :: draw ()
{
	int pad;
	pad = 20;
	
	if( side == 0 )
		rect.x = pad;
	
	if( side == 1 )
		rect.x = ofGetWidth() - rect.width - pad;
	
	rect.y = (int)( ( ofGetHeight() - rect.height ) * pos );
	
	ofSetColor( color );
	ofFill();
	ofRect( rect.x, rect.y, rect.width, rect.height );
}