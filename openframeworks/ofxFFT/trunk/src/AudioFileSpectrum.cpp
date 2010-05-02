/*
 *  AudioFileSpectrum.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 24/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioFileSpectrum.h"

AudioFileSpectrum :: AudioFileSpectrum()
{
	//
}

AudioFileSpectrum :: ~AudioFileSpectrum()
{
	//
}

void AudioFileSpectrum :: init( string fileName )
{
	audio = new ofSoundPlayer();
	audio->loadSound( fileName, false );
	audio->play();
	audio->setPosition( 0.0 );
	audio->setVolume( 1 );
	audio->setLoop( true );

	init();
}

void AudioFileSpectrum :: init( ofSoundPlayer *audioPtr )
{
	audio = audioPtr;
	
	init();
}

void AudioFileSpectrum :: init ()
{
	AudioAbstract :: init();
	
	float audioLengthInvFreq = audio->length / (float)audio->internalFreq;
	audioTotalFrames = floor( audioLengthInvFreq * ofGetFrameRate() );
	
	setFrameRateSync( false );
}

void AudioFileSpectrum :: update ()
{
	if( frameRateSync )
	{
		float position;
		position = ( ofGetFrameNum() - frameStart ) / (float)audioTotalFrames;
		
		audio->setPosition( position );
	}
	
	float *data;
	data = ofSoundGetSpectrum( getNoOfBands() );
	
	for( int i=0; i<getNoOfBands(); i++)
	{
		specData[ i ] = data[ i ];
	}
	
	AudioAbstract :: update();
}

/////////////////////////////////////////////
//	SETTERS / GETTERS.
/////////////////////////////////////////////

void AudioFileSpectrum :: setFrameRateSync( bool b )
{
	frameRateSync = b;
	
	frameStart = ofGetFrameNum();
}