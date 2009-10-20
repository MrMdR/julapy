/*
 *  ofxCvWarper.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 17/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_CV_WARPER_
#define _OFX_CV_WARPER_

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxResizeUtil.h"
#include "ofxMSAInteractiveObject.h"

#include <iostream>
#include <fstream>
#include <string>


class ofxCvWarper
{

public :
	
	ofxCvWarper();
	~ofxCvWarper();
	
	void setup( ofxCvImage *_srcImage, ofxCvImage *_dstImage );
	void setPosition( float x, float y );
	void warp();
	void draw();
	void reset();
	void onMouseDragged( ofMouseEventArgs &mouseArgs );
	
	void save( string fileName );
	void load( string fileName );
	
	ofxCvImage	*srcImage;
	ofxCvImage	*dstImage;
	
	ofRectangle	srcRect;
	ofRectangle dstRect;
	
	ofPoint		srcPoints[ 4 ];
	ofPoint		dstPoints[ 4 ];
	
	ofPoint		pos;
	
	ofxMSAInteractiveObject		anchors[ 4 ];
	int							anchorSize;
};

#endif