/*
 *  AudioAbstract.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef AUDIO_ABSTRACT_H
#define AUDIO_ABSTRACT_H

#include "ofMain.h"
#include "fft.h"

#define AUDIO_ABSTRACT_DEFAULT_RENDER_WIDTH		512
#define AUDIO_ABSTRACT_DEFAULT_RENDER_HEIGHT	256
#define AUDIO_ABSTRACT_DEFAULT_NO_OF_BANDS		128

struct AudioData
{
	int				size;
	
	vector<float>	data;
	vector<float>	dataNorm;
	vector<float>	dataMax;
	vector<float>	dataPeak;
	vector<int>		dataCut;
	
	float			maxDecay;
	float			peakDecay;
	float			peakAverage;
	float			cutThreshold;
	
	float			linearEQIntercept;
	float			linearEQSlope;
};

class AudioAbstract : public ofBaseApp
{

public :
	
	 AudioAbstract();
	~AudioAbstract();
	
	virtual void  init				( );
	virtual void  update			( );
	
	virtual void  draw				( int x, int y, int w, int h );
	virtual void  draw				( int w = AUDIO_ABSTRACT_DEFAULT_RENDER_WIDTH, int h = AUDIO_ABSTRACT_DEFAULT_RENDER_HEIGHT );
	
	virtual void  drawData			( const AudioData &audioData, int w, int h );
	virtual void  drawBg			( const AudioData &audioData, int w, int h );
	virtual void  drawGlitchData	( const AudioData &audioData, int w, int h );
	virtual void  drawFftData		( const AudioData &audioData, int w, int h );
	virtual void  drawFftNormData	( const AudioData &audioData, int w, int h );	
	virtual void  drawFftPeakData	( const AudioData &audioData, int w, int h );
	virtual void  drawThresholdLine ( const AudioData &audioData, int w, int h );
	
	virtual void  initFFT			( );
	virtual void  resetFFT			( );
	
	virtual void  initAudioData		( AudioData &audioData, int dataSize );
	virtual void  updateAudioData	( AudioData &audioData, float *dataNew );
	virtual void  mirrorAudioData	( AudioData &audioData );
	virtual void  resetAudioData	( AudioData &audioData );
	
	virtual void  setNoOfBands		( int value );
	virtual int   getNoOfBands		( );
	virtual void  setThreshold		( float value );
	virtual float getThreshold		( );
	virtual float getAveragePeak	( );
	virtual void  setPeakDecay		( float value );
	virtual float getPeakDecay		( );
	virtual void  setMaxDecay		( float value );
	virtual float getMaxDecay		( );
	virtual void  setMirrorData		( bool value );
	virtual void  setUseFftData		( bool value );
	
	virtual void  getFftData		( float *data, int length );
	virtual void  getFftPeakData	( float *data, int length );
	virtual void  getGlitchData		( int   *data, int length );
	
protected :	
	
	int		audioNoOfBands;
	int		audioNoOfBandsHalf;
	
	float	*specData;
	
	fft		myfft;
	float	*fftMagnitude;
	float	*fftPhase;
	float	*fftPower;
	float	*fftFreq;
	float	fftAveragePower;
	
	bool	bMirrorData;
	bool	bUseFftData;
	
	AudioData	rawData;
	AudioData	fftData;
	
	//-- render bits.
	
	int		renderBorder;
	int		renderSingleBandWidth;
	
};

#endif