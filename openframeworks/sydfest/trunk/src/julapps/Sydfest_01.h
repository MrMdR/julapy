/*
 *  Sydfest_01.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 22/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SYDFEST_01_H_
#define _SYDFEST_01_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxCirclePacker.h"
#include "ofxBox2d.h"
#include "ofxSimpleGuiToo.h"

class Sydfest_01 : public ofxJulapyApp
{
	
public :
	
	 Sydfest_01	();
	~Sydfest_01	();
	
	void setup		();
	void update		();
	void draw		();
	void drawDebug	();
	
	void initImage			();
	void initCirclePacker	();
	void initGui			();
	
	void keyReleased	( int key );

	ofxCirclePacker	circlePacker;
	float			circleRadiusMin;
	float			circleRadiusMax;
	float			circleGap;
	float			circleDeathGap;
	int				circleAddedPerFrame;
	bool			circleColorBounds;
	
	ofImage			imageMap;
	ofRectangle		imageMapRect;
	int				imageMapAlpha;
	bool			imageMapShow;

	ofImage			imageBounds;
	ofRectangle		imageBoundsRect;
	int				imageBoundsAlpha;
	bool			imageBoundsShow;
	
	int				bgColor;
	
	ofxSimpleGuiToo	gui;
	
};

#endif