/*
 *  ColorCycle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxScreen.h"
#include "ofxColorPicker.h"
#include "ColorRect.h"

class ColorCycle
{

public:
	
	 ColorCycle();
	~ColorCycle();
	
	void setScreenSize	( int w, int h );
	void setFrameRate	( int fr );
	
	void setup			();
	void update			();
	void draw			();
	
	void drawColorPickers	();
	
	void showPanel		();
	void hidePanel		();
	void togglePanel	();
	
	void down			( int x, int y );
	void drag			( int x, int y );
	void up				( int x, int y );
	
	ofxScreen		screen;
	int				frameRate;
	
	ofxColorPicker	colorPicker0;
	ofxColorPicker	colorPicker1;
	float			colorScale;
	
	ColorRect		rect;
	
	bool			bInputDown;
	ofPoint			inputPos1;
	ofPoint			inputPos2;
	ofPoint			inputVel;
	
	bool			bPanel;
	
};