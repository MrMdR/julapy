/*
 *  AudioFileSpectrum.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 24/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AUDIO_FILE_SPECTRUM_H_
#define AUDIO_FILE_SPECTRUM_H_

#include "ofMain.h"
#include "AudioAbstract.h"

class AudioFileSpectrum : public AudioAbstract
{

public :
	
	 AudioFileSpectrum();
	~AudioFileSpectrum();
	
	void  init				( string fileName );
	void  init				( ofSoundPlayer *audio );
	
	void  update			( );
	
	void  setFrameRateSync	( bool b );
	
	ofSoundPlayer*			audio;
	
private :
	
	void init				();
	
	int						audioTotalFrames;
	bool					frameRateSync;
	int						frameStart;
	
	
};

#endif