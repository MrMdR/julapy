/*
 *  Sydfest_04.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 28/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SYDFEST_04_H_
#define _SYDFEST_04_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxOpenCv.h"
#include "ofxCvWarper.h"
#include "ofxCvContourFinderToo.h"
#include "ofxContourAnalysis.h"
#include "ofxBox2d.h"
#include "ofxSimpleGuiToo.h"

class Sydfest_04 : public ofxJulapyApp
{

public :
	
	 Sydfest_04 ();
	~Sydfest_04 ();
	
	void setup		();
	void update		();
	void draw		();
	void drawDebug	();
	
	void keyReleased	( int key );
	void mousePressed	( int x, int y, int button );

	void initVideoGrabber		();
	void updateVideoGrabber		();
	void drawVideoGrabber		();
	void drawVideoGrabberWarper ();
	
	void initImage				();
	void drawImage				();

	void initContours			();
	void updateContours			();
	void drawContours			();
	
	void initBox2d				();
	void updateBox2d			();
	void drawBox2d				();
	
	void initGui				();
	
	ofVideoGrabber		videoGrabber;
	
	ofRectangle			videoGrabberLrgRect;
	ofRectangle			videoGrabberSmlRect;
	ofRectangle			videoGrabberSrcRect;
	ofRectangle			videoGrabberDstRect;
	
	bool				isVideoGrabberNewFrame;
	
	ofxCvColorImage		videoGrabberLrgImage;
	ofxCvColorImage		videoGrabberSmlImage;
	
	ofxCvGrayscaleImage	videoGrabberSrcImage;
	ofxCvGrayscaleImage videoGrabberDstImage;
	
	ofxCvWarper			videoGrabberWarper;
	
	ofxCvContourFinderToo	contourFinder;
	ofxContourAnalysis		contourAnalysis;
	
	ofxSimpleGuiToo		gui;
	
	ofImage				image;
	ofRectangle			imageRect;
	ofxCvColorImage		imageCol;
	ofxCvGrayscaleImage	imageGry;
	
	bool				bDrawDebug;
	float				threshold;
	
	ofxBox2d				box2d;
	vector<ofxBox2dCircle>	box2dCircles;
	vector<ofxBox2dLine>	box2dLineStrips;
};

#endif