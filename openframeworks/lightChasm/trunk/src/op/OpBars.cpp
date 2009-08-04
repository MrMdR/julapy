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
	setAudioAvgMin( 15 );
	
	fbo.allocate( width, height, true );
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
			bars.back().x		= ofRandom( 0, width );
			bars.back().height	= height;
		}
	}
}

void OpBars :: setAudioInData ( float *data )
{
	for( int i=0; i<bars.size(); i++ )
	{
		audioAverage[ i ] = data[ i ];
		
		if( audioAverage[ i ] < audioAvgMin )
		{
			audioAverage[ i ] = 0;
		}
		
		audioPeak[ i ] *= 0.96;
		
		if( audioPeak[ i ] < audioAverage[ i ] )
		{
			audioPeak[ i ] = audioAverage[ i ] * 3;
		}
		
		float aup = audioPeak[ i ] * 5;
		
		bars[ i ].width	+= ( aup - bars[ i ].width ) * 0.7;
		
		if( bars[ i ].width < 3 )
		{
			bars[ i ].width = 0;
		}
	}
}

void OpBars :: setAudioAvgMin( float avgMin )
{
	audioAvgMin = avgMin;
}

void OpBars :: update()
{
	
}

void OpBars :: drawToFBO()
{
	fbo.swapIn();
	fbo.setupScreenForMe();
	
	draw();
	
	fbo.swapOut();
	fbo.setupScreenForThem();
}

void OpBars :: drawFBOToScreen()
{
	ofSetColor( 255, 255, 255 );
	fbo.draw( 0, 0 );
}

void OpBars :: draw()
{
	ofFill();
	
	for( int i=0; i<bars.size(); i++ )
	{
		if( i % 1 == 0 )
		{
			ofSetColor( 255, 255, 255 );
		}
		else
		{
			ofSetColor( 0, 0, 0 );
		}
		
		ofRect
		(
			bars[ i ].x - bars[ i ].width * 0.5f,
			bars[ i ].y,
			bars[ i ].width,
			bars[ i ].height
		);
	}
}
