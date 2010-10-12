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

void AudioLiveSpectrum :: init ()
{
	AudioAbstract :: init();
	
	ofSoundStreamSetup
	(
		0,						// nOutputChannels
		2,						// nInputChannels
		this,					// ofBaseApp * OFSA
		44100,					// sampleRate
		getNoOfBands(),			// bufferSize
		1						// nBuffers
	);
}

void AudioLiveSpectrum :: audioReceived( float *data, int bufferSize, int nChannels )
{
	for( int i=0; i<getNoOfBands(); i++)
	{
		int j = i;
//		j *= 0.1;
		
		specData[ i ] = data[ j * 2 ];		// samples are "interleaved", so left channel data is every second one.
	}
}
