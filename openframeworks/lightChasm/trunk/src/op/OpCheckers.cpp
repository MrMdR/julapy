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
	ofFill();
	
	int cx = ceil( width * 0.5 / size );
	int cy = ceil( height * 0.5 / size );
	
	float dx = width * 0.5 - cx * size;
	float dy = height * 0.5 - cy * size;
	
	int ix = 0;
	int iy = 0;
	
	bool odd = true;
	if( ( cx % 2 == 0 && cy % 2 != 0 )  || ( cx % 2 != 0 && cy % 2 == 0 ) )
	{
		odd = false;
	}
	
	if( odd )
	{
		iy = 1;
	}
	
	for( int x=0; x<=cx*2; x++ )
	{
		for( int y=0; y<=cy*2; y++ )
		{
			if( iy % 2 == 0 )
			{
				ofSetColor( 0, 0, 0 );
			}
			else
			{
				ofSetColor( 255, 255, 255 );
			}
				
			ofRect
			(
				dx + x * size,
				dy + y * size,
				size,
				size
			);
			
			iy += 1;
		}
		
		if( ix % 2 == 0 )
		{
			iy = 1;
			if( odd )
				iy = 0;
		}
		else
		{
			iy = 0;
			if( odd )
				iy = 1;
		}
		
		ix += 1;
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
