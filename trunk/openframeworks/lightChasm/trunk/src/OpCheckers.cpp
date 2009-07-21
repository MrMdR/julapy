/*
 *  OpCheckers.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 10/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpCheckers.h"

OpCheckers :: OpCheckers()
{

}

OpCheckers :: ~OpCheckers()
{

}

void OpCheckers :: init( int w, int h )
{
	width	= w;
	height	= h;
	
	setSize( 10.0f );
	setSizeEase( 0.5f );
}

void OpCheckers :: update()
{

}

void OpCheckers :: draw()
{
	float dx = 0;
	float dy = 0;
	int xi = 0;
	int yi = 0;
	
	ofFill();
	
	while( dx < width + size )
	{
		while( dy < height + size )
		{
			if( yi % 2 == 0 )
			{
				ofSetColor( 0x000000 );
			}
			else
			{
				ofSetColor( 0xFFFFFF );
			}
				
			ofRect( dx, dy, size, size );
			
			dy += size;
			yi += 1;
		}
		
		dy = 0;
		if( xi % 2 == 0 )
		{
			yi = 1;
		}
		else
		{
			yi = 0;
		}
		
		dx += size;
		xi += 1;
	}
}

void OpCheckers :: setSize( float s )
{
	if( s < 1 )
	{
		s = 1;
	}
	
	size += ( s - size ) * sizeEase;
}

void OpCheckers :: setSizeEase( float value )
{
	sizeEase = value;
	
	if( sizeEase < 0 )
		sizeEase = 0;
	
	if( sizeEase > 1 )
		sizeEase = 1;
}
