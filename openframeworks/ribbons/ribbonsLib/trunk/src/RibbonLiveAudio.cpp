/*
 *  RibbonAudio.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonLiveAudio.h"

RibbonLiveAudio :: RibbonLiveAudio()
{
	left	= new float[ BUFFER_SIZE ];
	right	= new float[ BUFFER_SIZE ];
	
	for( int i=0; i<BUFFER_SIZE; i++ )
	{
		magnitudeCurr[ i ]	= 0;
		phase[ i ]			= 0;
		power[ i ]			= 0;
	}
	
//	ofSoundStreamSetup( 0, 2, this, 44100, BUFFER_SIZE, 4 );
}

RibbonLiveAudio :: ~RibbonLiveAudio()
{
	//
}

void RibbonLiveAudio :: scaleRibbon( float *ribbonPositionArray, float *ribbonDirectionArray, float *ribbonVertexArray, int ribbonIndex, int ribbonTotal, int trailIndex )
{
//	float audioScale = avgPowerScale * 10;
//	ribbonVertexArray[ 0 ] = ribbonPositionArray[ 0 ] - ribbonDirectionArray[ 0 ] * audioScale;
//	ribbonVertexArray[ 1 ] = ribbonPositionArray[ 1 ] - ribbonDirectionArray[ 1 ] * audioScale;
//	ribbonVertexArray[ 2 ] = ribbonPositionArray[ 2 ] - ribbonDirectionArray[ 2 ] * audioScale;
//	ribbonVertexArray[ 3 ] = ribbonPositionArray[ 3 ] + ribbonDirectionArray[ 0 ] * audioScale;
//	ribbonVertexArray[ 4 ] = ribbonPositionArray[ 4 ] + ribbonDirectionArray[ 1 ] * audioScale;
//	ribbonVertexArray[ 5 ] = ribbonPositionArray[ 5 ] + ribbonDirectionArray[ 2 ] * audioScale;
	
	for( int i=0; i<trailIndex - 1; i++ )
	{
		int magnitudeIndex = (int)( ribbonIndex / (float)( ribbonTotal - 1 ) * ( BUFFER_SIZE - 1 ) );
		float audioScale   = magnitudeCurr[ magnitudeIndex ] * 3 + 1;
//		float audioScale   = magnitude[ magnitudeIndex ] * 10 + 2;
		ribbonVertexArray[ i * 6 + 0 ] = ribbonPositionArray[ i * 3 + 0 ] - ribbonDirectionArray[ i * 3 + 0 ] * audioScale;
		ribbonVertexArray[ i * 6 + 1 ] = ribbonPositionArray[ i * 3 + 1 ] - ribbonDirectionArray[ i * 3 + 1 ] * audioScale;
		ribbonVertexArray[ i * 6 + 2 ] = ribbonPositionArray[ i * 3 + 2 ] - ribbonDirectionArray[ i * 3 + 2 ] * audioScale;
		ribbonVertexArray[ i * 6 + 3 ] = ribbonPositionArray[ i * 3 + 0 ] + ribbonDirectionArray[ i * 3 + 0 ] * audioScale;
		ribbonVertexArray[ i * 6 + 4 ] = ribbonPositionArray[ i * 3 + 1 ] + ribbonDirectionArray[ i * 3 + 1 ] * audioScale;
		ribbonVertexArray[ i * 6 + 5 ] = ribbonPositionArray[ i * 3 + 2 ] + ribbonDirectionArray[ i * 3 + 2 ] * audioScale;
	}
}

void RibbonLiveAudio :: audioReceived( float * input, int bufferSize, int nChannels )
{
	for( int i=0; i<bufferSize; i++ )
	{
		left[ i ]	= input[ i * 2 + 0 ];
		right[ i ]	= input[ i * 2 + 1 ];
	}

	myfft.powerSpectrum( 0, (int)( BUFFER_SIZE * 0.5 ), left, BUFFER_SIZE, &magnitude[0], &phase[0], &power[0], &avgPower );

	for( int i=0; i<BUFFER_SIZE; i++ )
	{
		magnitudeCurr[ i ] += ( magnitude[ i ] - magnitudeCurr[ i ] ) * 0.01;
//		magnitudeCurr[ i ] += ( magnitudeNorm[ i ].getNormalisedValue( magnitude[ i ] ) - magnitudeCurr[ i ] ) * 0.1;
	}
	
//	avgPowerScale += ( avgPowerNorm.getNormalisedValue( avgPower ) - avgPowerScale ) * 0.01;
	avgPowerScale = avgPowerNorm.getNormalisedValue( avgPower );
}

float RibbonLiveAudio :: getAveragePowerScale ()
{
//	return avgPowerScale;
	return avgPower;
}

int RibbonLiveAudio :: getBufferSize()
{
	return BUFFER_SIZE;
}