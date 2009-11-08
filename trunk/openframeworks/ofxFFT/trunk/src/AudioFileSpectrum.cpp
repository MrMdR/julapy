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
	AudioAbstract :: init( "" );
	
	audio.loadSound( fileName, false );
	audio.play();
	audio.setPosition( 0.0 );
	audio.setVolume( 1 );
	audio.setLoop( true );
	
	float audioLengthInvFreq = audio.length / (float)audio.internalFreq;
	audioTotalFrames = floor( audioLengthInvFreq * ofGetFrameRate() );
}

void AudioFileSpectrum :: update()
{
	if( frameRateSync )
	{
		float position = ( ofGetFrameNum() - frameStart ) / (float)audioTotalFrames;
		audio.setPosition( position );
	}
	
	specData = ofSoundGetSpectrum( audioNoOfBands );
	
	AudioAbstract :: update();
}

/////////////////////////////////////////////
//	SETTERS / GETTERS.
/////////////////////////////////////////////

void AudioFileSpectrum :: setPosition( float value )
{
	audio.setPosition( value );
}

float AudioFileSpectrum :: getPosition()
{
	return audio.getPosition();
}

void AudioFileSpectrum :: setVolume( float value )
{
	audio.setVolume( value );
}

float AudioFileSpectrum :: getVolume()
{
	return audio.volume;
}