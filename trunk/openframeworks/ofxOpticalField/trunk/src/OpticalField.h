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

#define		OPTICAL_FIELD_WIDTH		320
#define		OPTICAL_FIELD_HEIGHT	240

class OpticalField
{

public :
	
	OpticalField();
	~OpticalField();
	
	void init( const ofRectangle& srcRect, const ofRectangle& dstRect );
	void update( unsigned char *pixels );
	
	void setImageType( int value );
	void setMirror( bool mirrorV, bool mirrorH );
	
	void drawCurrentColorImage( );
	void drawCurrentGreyImage( );
	void drawDifference( );
	void drawOpticalFlow( float lineScale = 1000 );
	
	int getWidth();
	int getHeight();
	
	void reset();
	void getVelAtPixel( int x, int y, float *u, float *v );
	void getVelAtNorm( float x, float y, float *u, float *v );
	void getInteractionScale( float *v );
	
	ofRectangle				srcRect;
	ofRectangle				dstRest;

	ofxCvColorImage			sourceColImg;
	ofxCvColorImage			resizeColImg;
	
	ofxCvGrayscaleImage		sourceBwImg;
	ofxCvGrayscaleImage		resizeBwImg;
	
	ofxCvGrayscaleImage		greyNow;
	ofxCvGrayscaleImage		greyPrev;
	ofxCvGrayscaleImage		greyCurDiff;
	ofCvOpticalFlowLK		opticalFlow;
	
	int		imageType;
	bool	mirrorV;
	bool	mirrorH;
	
	float	opticalFlowMin;
	float	opticalFlowMax;
	float	opticalFlowScale;
	
	int		opticalFlowSize;
	int		opticalFlowBlur;
	int		sourceImgBlur;
	
	bool	showDifferenceImage;
};