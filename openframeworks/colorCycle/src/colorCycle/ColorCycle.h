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
#include "ofxDelaunay.h"
#include "ColorRect.h"
#include "ColorPhysics.h"
#include "ColorTriangle.h"
#include "ColorPanel.h"

class ColorCycle
{

public:
	
	 ColorCycle();
	~ColorCycle();
	
	void setScreenSize	( int w, int h );
	void setFrameRate	( int fr );
	
	void setGravity		( float gx, float gy );
	
	void setup			();
	void update			();
	void draw			();
	
	void updatePhysics			();
	void updateDelaunay			();
	void updateTriangles		();
	ofColor interpolateColor	( const ofColor& c1, const ofColor& c2, float p );
	
	void drawColorPickers		();
	void drawTriangles			();
	
	void showPanel		();
	void hidePanel		();
	void togglePanel	();
	
	void resetJoints	();
	
	void down			( int x, int y );
	void drag			( int x, int y );
	void up				( int x, int y );
	
	ofxScreen		screen;
	int				frameRate;
	
	ofxColorPicker	colorPicker0;
	ofxColorPicker	colorPicker1;
	float			colorScale;
	ofColor			upperColor;
	ofColor			lowerColor;
	ofColor			white;
	ofColor			black;
	
	ColorPanel		panel;
	
	ColorRect		rect;
	
	ColorPhysics	physics;
	
	bool			bInputDown;
	ofPoint			inputPos1;
	ofPoint			inputPos2;
	ofPoint			inputVel;
	
	ofxDelaunay				delaunay;
	vector<ColorTriangle*>	triangles;
};