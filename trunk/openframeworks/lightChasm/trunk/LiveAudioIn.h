/*
 *  LiveAudioIn.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 9/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"

#define		BUFFER_SIZE		512

class LiveAudioIn
{

public :
	
	LiveAudioIn();
	~LiveAudioIn();
	
	void init();
	void update();
	void draw();
	
	FFTOctaveAnalyzer	fftAnalyzer;

	float	*left;
	float	*right;
	float	*averages;
	float	*peaks;
	
	int		averagesPerOctave;
	int		averagesTotal;
	
private :

	int 	bufferCounter;
	fft		myfft;
	
	float magnitude[ BUFFER_SIZE ];
	float phase[ BUFFER_SIZE ];
	float power[ BUFFER_SIZE ];
	float freq[ BUFFER_SIZE / 2 ];
	
	float avgPower;
	
};