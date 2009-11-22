/*
 *  Blobs02.h
 *  julapy_softbodies
 *
 *  Created by lukasz karluk on 22/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BLOBS_02_H_
#define _BLOBS_02_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"
#include "ofxCvWarper.h"
#include "OpticalField.h"

#include "AudioAbstract.h"
#include "AudioLiveSpectrum.h"

#ifndef _BLOB_H_
#define _BLOB_H_

class Blob
{
public :
	vector <ofxBox2dCircle>		nodes;
	vector <ofxBox2dJoint>		nodeJoints;
};

#endif

class Blobs02 : public ofxJulapyApp
{
		
public :
		
	Blobs02();
	~Blobs02();
		
	void setup();
	void update();
	void draw();
		
	void initBox2d();
	void initAudioIn();
		
	void addBlob( float bx, float by, float radius, int nodesTotal );
		
	void updateBox2d();
	void updateAudio();
		
	void drawBox2d();
	void drawAudio();
		
	void keyReleased( int key );
		
	bool bDrawNodes;
	bool bDrawJoints;
	bool bDrawCurves;
		
	ofxBox2d			box2d;			  //	the box2d world
	vector <Blob>		blobs;
	
	AudioAbstract		*audio;
};

#endif