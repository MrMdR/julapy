/*
 *  PixelFlow.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 6/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"

class PixelFlow
{

public:
	
	 PixelFlow();
	~PixelFlow();
	
	void setPixels ( unsigned char* pixels, int width, int height, int type );
	
	ofxVec2f getVectorAt( const ofPoint& point, int rangeX, int rangeY );
	ofColor	 getColourAt( const ofPoint& point );
	void	 getPixelsAt( const ofPoint& point, int rangeX, int rangeY, ofImage& img );
	
	unsigned char* pixels;
	int pixelsW;
	int pixelsH;
	int pixelsType;
	int pixelsBpp;
};