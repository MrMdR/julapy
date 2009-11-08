/*
 *  AudioLiveSpectrum.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioLiveSpectrum.h"

AudioLiveSpectrum :: AudioLiveSpectrum()
{
	//
}

AudioLiveSpectrum :: ~AudioLiveSpectrum()
{
	//
}

void AudioLiveSpectrum :: init( string fileName )
{
	AudioAbstract :: init( "" );
	
	setNoOfBands( 128 );
	
	ofSoundStreamSetup( 0, 2, this, 44100, getNoOfBands() * 2, 4 );
}

void AudioLiveSpectrum :: audioReceived( float *data, int bufferSize, int nChannels )
{
	for( int i=0; i<bufferSize; i++)
	{
		specData[ i ] = data[ i * 2 ];		// only using the left channel.
	}
	
	update();
}
