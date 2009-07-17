/*
 *  RibbonAudio.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonAudio.h"

RibbonAudio :: RibbonAudio()
{
	left	= new float[ BUFFER_SIZE ];
	right	= new float[ BUFFER_SIZE ];
}

RibbonAudio :: ~RibbonAudio()
{
	//
}

void RibbonAudio :: reactRibbonWidth(  float *ribbonVertexArray, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize )
{
	float avg_power = 0.0f;
	
	myfft.powerSpectrum( 0, (int)( BUFFER_SIZE * 0.5 ), left, BUFFER_SIZE, &magnitude[0], &phase[0], &power[0], &avg_power );
	
	int n = (int)( BUFFER_SIZE * 0.5 );
	for( int i=0; i<n; i++ )
	{
		int j = (int)( i / (float)( n - 1 ) * ( ribbonSize - 1 ) );
	}
}

void RibbonAudio :: audioReceived( float * input, int bufferSize, int nChannels )
{
	for( int i = 0; i < bufferSize; i++ )
	{
		left[ i ]	= input[ i * 2 + 0 ];
		right[ i ]	= input[ i * 2 + 1 ];
	}
}

int RibbonAudio :: getBufferSize()
{
	return BUFFER_SIZE;
}