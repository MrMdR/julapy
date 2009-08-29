/*
 *  RibbonAudio.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonAudio.h"

RibbonAudio :: RibbonAudio() {};
RibbonAudio :: ~RibbonAudio() {};

void RibbonAudio :: setup( AudioAbstract *a )
{
	audio = a;
}

void RibbonAudio :: init( string fileName )
{
	audio->init( fileName );
}

void RibbonAudio :: resetFFT()
{
	audio->resetFFT();
}

void RibbonAudio :: update()
{
	audio->update();
}

void RibbonAudio :: draw( int width, int height )
{
	audio->draw( width, height );
}

void RibbonAudio :: setNoOfBands( int value )
{
	audio->setNoOfBands( value );
}

int RibbonAudio :: getNoOfBands()
{
	return audio->getNoOfBands();
}

void RibbonAudio :: setFrameRateSync( bool b )
{
	audio->setFrameRateSync( b );
}

void RibbonAudio :: setThreshold( float value )
{
	audio->setThreshold( value );
}

float RibbonAudio :: getThreshold()
{
	return audio->getThreshold();
}

float RibbonAudio :: getAveragePeak()
{
	return audio->getAveragePeak();
}

float RibbonAudio :: getAveragePeakNorm()
{
	return audio->getAveragePeakNorm();
}

void RibbonAudio :: setPosition( float value )
{
	audio->setPosition( value );
}

float RibbonAudio :: getPosition()
{
	return audio->getPosition();
}

void RibbonAudio :: getGlitchData( int *data, int length )
{
	audio->getGlitchData( data, length );
}

//void RibbonAudio :: scaleRibbon( float *ribbonPositionArray, float *ribbonDirectionArray, float *ribbonVertexArray, int ribbonIndex, int ribbonTotal, int trailIndex )
//{
//	//	float audioScale = avgPowerScale * 10;
//	//	ribbonVertexArray[ 0 ] = ribbonPositionArray[ 0 ] - ribbonDirectionArray[ 0 ] * audioScale;
//	//	ribbonVertexArray[ 1 ] = ribbonPositionArray[ 1 ] - ribbonDirectionArray[ 1 ] * audioScale;
//	//	ribbonVertexArray[ 2 ] = ribbonPositionArray[ 2 ] - ribbonDirectionArray[ 2 ] * audioScale;
//	//	ribbonVertexArray[ 3 ] = ribbonPositionArray[ 3 ] + ribbonDirectionArray[ 0 ] * audioScale;
//	//	ribbonVertexArray[ 4 ] = ribbonPositionArray[ 4 ] + ribbonDirectionArray[ 1 ] * audioScale;
//	//	ribbonVertexArray[ 5 ] = ribbonPositionArray[ 5 ] + ribbonDirectionArray[ 2 ] * audioScale;
//	
//	for( int i=0; i<trailIndex - 1; i++ )
//	{
//		int fftDataIndex = (int)( ribbonIndex / (float)( ribbonTotal - 1 ) * ( audioNoOfBands - 1 ) );
//		float audioScale   = fftDataPeak[ fftDataIndex ] * 30 + 1;
//		ribbonVertexArray[ i * 6 + 0 ] = ribbonPositionArray[ i * 3 + 0 ] - ribbonDirectionArray[ i * 3 + 0 ] * audioScale;
//		ribbonVertexArray[ i * 6 + 1 ] = ribbonPositionArray[ i * 3 + 1 ] - ribbonDirectionArray[ i * 3 + 1 ] * audioScale;
//		ribbonVertexArray[ i * 6 + 2 ] = ribbonPositionArray[ i * 3 + 2 ] - ribbonDirectionArray[ i * 3 + 2 ] * audioScale;
//		ribbonVertexArray[ i * 6 + 3 ] = ribbonPositionArray[ i * 3 + 0 ] + ribbonDirectionArray[ i * 3 + 0 ] * audioScale;
//		ribbonVertexArray[ i * 6 + 4 ] = ribbonPositionArray[ i * 3 + 1 ] + ribbonDirectionArray[ i * 3 + 1 ] * audioScale;
//		ribbonVertexArray[ i * 6 + 5 ] = ribbonPositionArray[ i * 3 + 2 ] + ribbonDirectionArray[ i * 3 + 2 ] * audioScale;
//	}
//}