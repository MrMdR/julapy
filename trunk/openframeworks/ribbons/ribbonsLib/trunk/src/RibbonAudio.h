/*
 *  RibbonAudio.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_AUDIO_H_
#define _RIBBON_AUDIO_H_

#include "AudioAbstract.h"

class RibbonAudio : public AudioAbstract
{

public :
	
	RibbonAudio();
	~RibbonAudio();
	
	void  setup( AudioAbstract *audio );
	
	void  init( string fileName );
	void  update();
	void  draw( int width=0, int height=0 );
	
	void  resetFFT();
	
	void  setNoOfBands( int value );
	int   getNoOfBands();
	void  setFrameRateSync( bool b );
	void  setThreshold( float value );
	float getThreshold();
	float getAveragePeak();
	float getAveragePeakNorm();
	
	void  setPosition( float value );
	float getPosition();
	
	void  getGlitchData( int *data, int length );
	
	AudioAbstract *audio;
	
};

#endif