/*
 *  AudioLiveSpectrum.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AUDIO_LIVE_SPECTRUM_H_
#define AUDIO_LIVE_SPECTRUM_H_

#include "ofMain.h"
#include "AudioAbstract.h"

class AudioLiveSpectrum : public AudioAbstract
{
		
public :
		
	 AudioLiveSpectrum();
	~AudioLiveSpectrum();
		
	void init			( );
	void audioReceived	( float *data, int bufferSize, int nChannels );
};

#endif