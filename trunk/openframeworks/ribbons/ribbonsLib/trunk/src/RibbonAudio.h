/*
 *  RibbonAudio.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RIBBON_AUDIO
#define RIBBON_AUDIO

#include "fft.h"
#include "DataNormaliser.h"

#define BUFFER_SIZE		256

class RibbonAudio
{
	
public :
	RibbonAudio();
	~RibbonAudio();
	
	void reactRibbonWidth( float *ribbonVertexArray, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize );
	void audioReceived( float * input, int bufferSize, int nChannels );
	int	 getBufferSize();
	
private:	
	float	*left;
	float	*right;
	fft		myfft;
	
	float magnitude[ BUFFER_SIZE ];
	float phase[ BUFFER_SIZE ];
	float power[ BUFFER_SIZE ];
	
	DataNormaliser magScaled[ BUFFER_SIZE ];
};

#endif