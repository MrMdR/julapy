/*
 *  Blobs01.h
 *  julapy_softbodies
 *
 *  Created by lukasz karluk on 22/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BLOBS_01_H_
#define _BLOBS_01_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"
#include "ofxCvWarper.h"
#include "OpticalField.h"

#ifndef _BLOB_H_
#define _BLOB_H_

class Blob
{
public :
	vector <ofxBox2dCircle>		nodes;
	vector <ofxBox2dJoint>		nodeJoints;
};

#endif

class Blobs01 : public ofxJulapyApp
{

public :
	
	Blobs01();
	~Blobs01();
	
	void setup();
	void update();
	void draw();
	
	void initBox2d();
	void initVideoGrabber();
	void initOpticalField();
	
	void addBlob( float bx, float by, float radius, int nodesTotal );
	
	void updateVideoGrabber();
	void updateOpticalField();
	void updateBox2d();
	
	void drawOpticalField();
	void drawVideoGrabberWarper();
	void drawBox2d();
	void drawDebug();
	
	void keyReleased( int key );
	
	bool bDrawNodes;
	bool bDrawJoints;
	bool bDrawCurves;
	
	ofxBox2d			box2d;			  //	the box2d world
	vector <Blob>		blobs;
	
	ofVideoGrabber		videoGrabber;
	
	ofRectangle			videoGrabberLrgRect;
	ofRectangle			videoGrabberSmlRect;
	ofRectangle			videoGrabberSrcRect;
	ofRectangle			videoGrabberDstRect;
	
	ofxCvColorImage		videoGrabberLrgImage;
	ofxCvColorImage		videoGrabberSmlImage;
	ofxCvGrayscaleImage	videoGrabberSrcImage;
	ofxCvGrayscaleImage videoGrabberDstImage;
	
	ofxCvWarper			videoGrabberWarper;
	
	bool				isVideoGrabberNewFrame;
	int					camWidth;
	int					camHeight;
	int					camWidthHalf;
	int					camHeightHalf;
	
	OpticalField		opticalField;
};

#endif