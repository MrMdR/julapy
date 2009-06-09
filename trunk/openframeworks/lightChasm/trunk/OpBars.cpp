/*
 *  OpBars.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 9/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpBars.h"

OpBars :: OpBars()
{

}

OpBars :: ~OpBars()
{

}

void OpBars :: init( int w, int h )
{
	width		= w;
	height		= h;

	barsTotal	= 0;

	setNumberOfBars( BARS_MAX );
}

void OpBars :: setNumberOfBars( int value )
{
	barsTotal = value;
	
	if( barsTotal < bars.size() )
	{
		for( int i=barsTotal; i<bars.size(); i++ )
		{
			bars.erase( bars.begin() + i );
		}
	}
	
	if( barsTotal > bars.size() )
	{
		while( bars.size() <= barsTotal )
		{
			bars.push_back( ofRectangle() );
		}
	}
}

void OpBars :: setAudioInData ( float *data )
{
	for( int i=0; i<bars.size(); i++ )
	{
		audioInData[ i ] = data[ i ];
		
		bars[ i ].x			= i * width / bars.size();
		bars[ i ].width		= width / bars.size();
		bars[ i ].height	= audioInData[ i ] * 8;
	}
}

void OpBars :: update()
{
	
}

void OpBars :: draw()
{
	ofFill();
	
	for( int i=0; i<bars.size(); i++ )
	{
		if( i % 1 == 0 )
		{
			ofSetColor( 0xFFFFFF );
		}
		else
		{
			ofSetColor( 0x000000 );
		}
		
		ofRect( bars[ i ].x, bars[ i ].y, bars[ i ].width, bars[ i ].height );
	}
}
