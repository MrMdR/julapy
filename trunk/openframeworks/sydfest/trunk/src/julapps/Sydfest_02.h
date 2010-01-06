/*
 *  Sydfest_02.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 23/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SYDFEST_02_H_
#define _SYDFEST_02_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxCirclePacker.h"
#include "ofxBox2d.h"
#include "ofxSimpleGuiToo.h"
#include "ofxShapes.h"

class Sydfest_02 : public ofxJulapyApp
{
	
public :
	
	 Sydfest_02	();
	~Sydfest_02	();
	
	void setup		();
	void update		();
	void draw		();
	void drawDebug	();
	
	ofxCirclePacker	circlePacker;
	vector<Circle>	*circles;
	
	ofxBox2d				box2d;
	vector<ofxBox2dCircle>	box2dCircles;
	vector<ofxBox2dLine>	box2dLineStrips;
	
	ofxShapes		shapes;

};

#endif