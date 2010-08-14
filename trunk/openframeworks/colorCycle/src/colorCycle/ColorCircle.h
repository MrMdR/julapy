/*
 *  ColorCircle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxVec2f.h"
#include "ofxScreen.h"

class ColorCircle : public ofxBox2dCircle
{
	
public:
	
	 ColorCircle();
	~ColorCircle();
	
	void setScreen	( ofxScreen screen );
	void setColor	( const ofColor& color );
	
	void init	();
	void update	();
	void draw	();
	
	void drawFill		();
	void drawStroke		();
	void drawColorLine	();
	
	ofxScreen	screen;
	
	ofColor		color;
	ofPoint		pos;
	ofPoint		posNorm;
	ofPoint		posOffset;
	ofPoint		posColor;
};