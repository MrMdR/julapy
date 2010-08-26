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
#include "ofxEaseValue.h"
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
	bool checkTriangleHit		( float x, float y, int id );
	ofColor interpolateColor	( const ofColor& c1, const ofColor& c2, float p );
	
	void drawColorPickers		();
	void drawTriangles			();
	
	void addCircle			();
	void removeCircle		();
	void shuffle			();
	void colorSelectMode	();
	
	void down			( int x, int y, int id = -1 );
	void drag			( int x, int y, int id = -1 );
	void up				( int x, int y, int id = -1 );
	void inputCheck		();
	
	ofxScreen		screen;
	int				frameRate;
	
	ofxColorPicker	colorPicker0;
	ofxColorPicker	colorPicker1;
	float			colorScale;
	ofxEaseValue	rndColorAngle0;
	ofxEaseValue	rndColorAngle1;
	ofColor			upperColor;
	ofColor			lowerColor;
	ofColor			white;
	ofColor			black;
	
	ColorPanel		panel;
	
	ColorRect		rect;
	
	ColorPhysics	physics;

	bool			bColorSelectMode;
	bool			bColorSelectModeOnBgClick;
	bool			bColorSelectTimeout;
	int				colorSelectTimeout;
	
	ofPoint			inputPos1;
	ofPoint			inputPos2;
	ofPoint			inputVel;
	int				inputLastID;
	bool			bInputDown;
	
	ofxDelaunay				delaunay;
	vector<ColorTriangle*>	triangles;
	ofxEaseValue			triColorScale;
	
#ifdef TARGET_OF_IPHONE
	// sound vector containers.
#else
	vector<ofSoundPlayer*>	spBackground;
	vector<ofSoundPlayer*>	spMeshDrag;
	vector<ofSoundPlayer*>	spPointAdd;
	vector<ofSoundPlayer*>	spPointRemove;
	vector<ofSoundPlayer*>	spPointCollide;
	vector<ofSoundPlayer*>	spPointShuffle;
#endif
};