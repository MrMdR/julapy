/*
 *  RibbonAudio.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_LIVE_AUDIO_H_
#define _RIBBON_LIVE_AUDIO_H_

#include "ofMain.h"
#include "fft.h"
#include "DataNormaliser.h"

#define BUFFER_SIZE		256

class RibbonLiveAudio
{
	
public :
	
	RibbonLiveAudio();
	~RibbonLiveAudio();
	
	void scaleRibbon( float *ribbonPositionArray, float *ribbonDirectionArray, float *ribbonVertexArray, int ribbonIndex, int ribbonTotal, int trailIndex );
	void audioReceived( float *input, int bufferSize, int nChannels );
	float getAveragePowerScale();
	int	 getBufferSize();
	
private:	
	
	float	*left;
	float	*right;
	fft		myfft;
	
	float magnitude[ BUFFER_SIZE ];
	float phase[ BUFFER_SIZE ];
	float power[ BUFFER_SIZE ];
	
	DataNormaliser magnitudeNorm[ BUFFER_SIZE ];
	float magnitudeCurr[ BUFFER_SIZE ];
	DataNormaliser avgPowerNorm;
	float avgPower;
	float avgPowerScale;
};

#endif