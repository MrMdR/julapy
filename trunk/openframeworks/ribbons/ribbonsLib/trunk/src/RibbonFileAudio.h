/*
 *  RibbonAudio.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_FILE_AUDIO_H_
#define _RIBBON_FILE_AUDIO_H_

#include "ofMain.h"
#include "fft.h"
#include "DataNormaliser.h"

#define BUFFER_SIZE		256

class RibbonFileAudio
{
	
public :
	
	RibbonFileAudio();
	~RibbonFileAudio();
	
	void init();
	void update();
	void scaleRibbon( float *ribbonPositionArray, float *ribbonDirectionArray, float *ribbonVertexArray, int ribbonIndex, int ribbonTotal, int trailIndex );
	void setFrameRateSync( bool b );
	
private:	
	
	ofSoundPlayer audio;
	int audioTotalFrames;
	int audioNoOfBands;
	
	bool frameRateSync;
	
	float *fftData;
	float *fftDataSmoothed;
};

#endif