/*
 *  Tracker.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 13/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef	TRACKER
#define TRACKER

#define USE_VIDEO_INPUT				// comment out to use camera input.
#define USE_POINT_GREY_CAMERA		// comment out to use normal camera that does not require IIDC lib.

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBlobUtil.h"
#include "ofxCvWarper.h"

class Tracker
{

public :
	
	 Tracker();
	~Tracker();
	
	void setup				();
	void update				();
	void draw				();
	
	void initVideo			();
	void initVideoGrabber	();
	void initVideoPlayer	();
	
	void updateVideoGrabber	();
	void updateVideoPlayer	();
	void updateTracking		();
	void updateBackground	();
	int  updateContours		();
	void updateCameraPos	( int x, int y );
	void updateRenderArea	( ofRectangle &area );
	
	
	bool					bUpdateCameraPos;
	
	//--
	
	ofPoint					screenPos;
	ofRectangle				renderArea;
	
#ifdef USE_VIDEO_INPUT	
	ofVideoPlayer			videoPlayer;
#else
#ifdef USE_POINT_GREY_CAMERA	
	ofxVideoGrabber			videoGrabber;
#else
	ofVideoGrabber			videoGrabber;
#endif
#endif
	
	ofRectangle				videoLrgRect;
	ofRectangle				videoSmlRect;
	
	bool					isVideoNewFrame;
	
	ofxCvGrayscaleImage		videoLrgImage;
	ofxCvGrayscaleImage		videoSmlImage;
	
	//--
	
	ofxCvWarper				cvWarper;
	ofxCvGrayscaleImage		cvImageWarp;
	ofxCvGrayscaleImage		cvImageBg;
	ofxCvGrayscaleImage		cvImageDiff;
	ofxCvGrayscaleImage		cvImageThsh;
	
	int						threshold_1;
	int						threshold_2;
	int						blur;
	
	ofxCvContourFinder		contourFinder;
	ofxCvBlobUtil			blobUtil;
	vector<ofxCvBlob>		blobs;
	
	ofPoint					cameraPosition;
	float					cameraRadius;	
};

#endif