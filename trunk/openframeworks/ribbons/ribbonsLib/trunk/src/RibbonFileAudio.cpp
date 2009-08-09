/*
 *  RibbonAudio.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonFileAudio.h"

RibbonFileAudio :: RibbonFileAudio()
{
	//
}

RibbonFileAudio :: ~RibbonFileAudio()
{
	//
}

void RibbonFileAudio :: init()
{
	audio.loadSound( "music/autechre_ep7_06_dropp.mp3", false );
//	audio.loadSound( "music/radioead_06_I_Am_Citizen_Insane.mp3", false );
	audio.play();
	audio.setPosition( 0.0 );
	audio.setVolume( 1 );
	
	float audioLengthInvFreq = audio.length / (float)audio.internalFreq;
	audioTotalFrames = floor( audioLengthInvFreq * ofGetFrameRate() );
	audioNoOfBands   = 128;
	
	fftData         = new float[ audioNoOfBands ];
	fftDataSmoothed = new float[ audioNoOfBands ];
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		fftData[ i ]         = 0;
		fftDataSmoothed[ i ] = 0;
	}
	
	setFrameRateSync( false );
}

void RibbonFileAudio :: update()
{
	if( frameRateSync )
	{
		float position = ofGetFrameNum() / (float)audioTotalFrames;
		audio.setPosition( position );
	}
	
	fftData = ofSoundGetSpectrum( audioNoOfBands );
	for( int i=0; i<audioNoOfBands; i++ )
	{
		fftDataSmoothed[ i ] *= 0.96;
		if( fftDataSmoothed[ i ] < fftData[ i ] )
			fftDataSmoothed[ i ] = fftData[ i ];
	}
	
}

void RibbonFileAudio :: scaleRibbon( float *ribbonPositionArray, float *ribbonDirectionArray, float *ribbonVertexArray, int ribbonIndex, int ribbonTotal, int trailIndex )
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
		int fftDataIndex = (int)( ribbonIndex / (float)( ribbonTotal - 1 ) * ( audioNoOfBands - 1 ) );
		float audioScale   = fftDataSmoothed[ fftDataIndex ] * 30 + 1;
		ribbonVertexArray[ i * 6 + 0 ] = ribbonPositionArray[ i * 3 + 0 ] - ribbonDirectionArray[ i * 3 + 0 ] * audioScale;
		ribbonVertexArray[ i * 6 + 1 ] = ribbonPositionArray[ i * 3 + 1 ] - ribbonDirectionArray[ i * 3 + 1 ] * audioScale;
		ribbonVertexArray[ i * 6 + 2 ] = ribbonPositionArray[ i * 3 + 2 ] - ribbonDirectionArray[ i * 3 + 2 ] * audioScale;
		ribbonVertexArray[ i * 6 + 3 ] = ribbonPositionArray[ i * 3 + 0 ] + ribbonDirectionArray[ i * 3 + 0 ] * audioScale;
		ribbonVertexArray[ i * 6 + 4 ] = ribbonPositionArray[ i * 3 + 1 ] + ribbonDirectionArray[ i * 3 + 1 ] * audioScale;
		ribbonVertexArray[ i * 6 + 5 ] = ribbonPositionArray[ i * 3 + 2 ] + ribbonDirectionArray[ i * 3 + 2 ] * audioScale;
	}
}

void RibbonFileAudio :: setFrameRateSync( bool b )
{
	frameRateSync = b;
}
