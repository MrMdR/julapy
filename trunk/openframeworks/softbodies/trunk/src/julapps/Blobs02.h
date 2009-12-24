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
#include "ofxVectorMath.h"
#include "ofxSimpleGuiToo.h"

#include "AudioAbstract.h"
#include "AudioLiveSpectrum.h"

#include "Blobs.h"

class Blobs02 : public ofxJulapyApp
{
		
public :
		
	Blobs02();
	~Blobs02();
		
	void setup();
	void update();
	void draw();
	void drawDebug();
		
	void initBox2d();
	void initAudioIn();
	void initGui();
		
	void addBlob( float bx, float by, float radius, int nodesTotal );
	void addBall( float bx, float by, float radius );
		
	void updateBox2d();
	void updateAudio();
		
	void drawBox2d();
	void drawBlobs();
	void drawBalls();
	void drawAudio();
		
	void keyReleased( int key );
		
	bool bDrawNodes;
	bool bDrawJoints;
	bool bDrawCurves;
		
	ofxSimpleGuiToo		gui;
	
	ofxBox2d			box2d;			  //	the box2d world
	vector <Blob>		blobs;
	vector <Ball>		balls;
	
	AudioLiveSpectrum	audio;
	
	float				blobScale;
	float				blobNodeDaming;
	float				blobNodeEase;
};

#endif