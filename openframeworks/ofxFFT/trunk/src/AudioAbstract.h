/*
 *  AudioAbstract.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AUDIO_ABSTRACT_H_
#define _AUDIO_ABSTRACT_H_

#include "ofMain.h"
#include "DataNormaliser.h"
#include "fft.h"

class AudioAbstract : public ofBaseApp
{

public :
	
	AudioAbstract();
	~AudioAbstract();
	
	virtual void  init( string fileName = NULL );
	virtual void  update();
	virtual void  draw( int width=0, int height=0 );
	
	virtual void  resetFFT();
	
	virtual void  setNoOfBands( int value );
	virtual int   getNoOfBands();
	virtual void  setFrameRateSync( bool b );
	virtual void  setThreshold( float value );
	virtual float getThreshold();
	virtual float getAveragePeak();
	virtual float getAveragePeakNorm();
	virtual void  setPeakDecay ( float value );
	virtual float getPeakDecay ();
	virtual void  setMaxDecay ( float value );
	virtual float getMaxDecay ();
	
	virtual void  getFftData ( float *data, int length );
	virtual void  getFftPeakData ( float *data, int length );
	virtual void  getGlitchData( int *data, int length );
	
	virtual	void  setPosition( float value );
	virtual float getPosition();
	
	virtual void  setVolume( float value );
	virtual float getVolume( );
	
	int audioTotalFrames;
	int audioNoOfBands;
	int audioNoOfBandsHalf;
	
	float	*specData;
	
	fft		myfft;
	float	*fftMagnitude;
	float	*fftPhase;
	float	*fftPower;
	float	*fftFreq;
	
	float	*fftData;
	float	*fftDataMax;
	float	*fftDataPeak;
	int		*fftDataSwitch;
	
	float	fftAveragePower;
	float	fftDataPeakDecay;
	float	fftDataMaxDecay;
	float	fftAveragePeak;
	DataNormaliser fftAveragePeakNorm;
	
	float	threshold;
	
	bool frameRateSync;
	int  frameStart;
	
	float linearEQIntercept;
	float linearEQSlope;
	
	int defaultRenderWidth;
	int defaultRenderHeight;
	
};

#endif