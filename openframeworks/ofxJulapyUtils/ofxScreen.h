/*
 *  ofxScreen.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class ofxScreen
{

public:
	
	ofxScreen	();
	ofxScreen	( int w, int h );
	~ofxScreen	();
	
	void set	( int w, int h );
	
	int screenWidth;
	int screenHeight;
	int screenMax;
	int screenMin;
	int screenArea;
	float screenDiagonal;
	
};