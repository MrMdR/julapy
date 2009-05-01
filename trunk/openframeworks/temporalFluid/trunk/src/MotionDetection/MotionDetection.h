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
#include "ofCvMain.h"
#include "ofCvOpticalFlowLK.h"
#include "ofCvOpticalFlowBM.h"

class MotionDetection
{

public :
	
	MotionDetection();
	~MotionDetection();
	
	void init( int w, int h );
	void update();
	void draw( int x, int y, int w, int h );
	void reset();
	void getVelAtPixel( int x, int y, float *u, float *v );
	void getVelAtNorm( float x, float y, float *u, float *v );
	void takeBgSnapshot();

	ofVideoGrabber			cam;
	int						camWidth;
	int						camHeight;
	
	int						camVelocityBlur;
	
	ofCvColorImage			colorImg;
	ofCvGrayscaleImage		greyNow;
	ofCvGrayscaleImage		greyBg;
	ofCvGrayscaleImage		greyBgDiff;
	ofCvGrayscaleImage		greyPrev;
	ofCvGrayscaleImage		greyCurDiff;
	
	ofCvOpticalFlowLK		opticalFlow;
	
	Boolean					saveBg;
	Boolean					newFrame;
	int						frameCounter;
};