/*
 *  MotionDetection.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxCvMain.h"
#include "ofCvOpticalFlowLK.h"
#include "ofCvOpticalFlowBM.h"

#define		WIDTH	320
#define		HEIGHT	240

class OpticalField
{

public :
	
	OpticalField();
	~OpticalField();
	
	void init( int w, int h );
	void update( unsigned char *pixels );
	
	void drawCurrentColorImage( int x, int y, int w, int h );
	void drawCurrentGreyImage( int x, int y, int w, int h );
	void drawDifference( int x, int y, int w, int h );
	void drawOpticalFlow( int x, int y, int w, int h );
	
	void reset();
	void getVelAtPixel( int x, int y, float *u, float *v );
	void getVelAtNorm( float x, float y, float *u, float *v );

	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage		greyNow;
	ofxCvGrayscaleImage		greyPrev;
	ofxCvGrayscaleImage		greyCurDiff;
	ofCvOpticalFlowLK		opticalFlow;
};