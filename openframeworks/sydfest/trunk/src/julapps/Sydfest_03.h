/*
 *  Sydfest_03.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 23/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SYDFEST_03_H_
#define _SYDFEST_03_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxCirclePacker.h"
#include "ofxBox2d.h"
#include "ofxSimpleGuiToo.h"
#include "ofxShapes.h"
#include "ofxBounce.h"

#ifndef _CIRCLE_ANIM_
#define _CIRCLE_ANIM_

struct CircleAnim
{
	ofPoint		loc;
	int			color;
	float		radius;
	float		radiusMax;
	float		radiusEase;
	float		frameDelay;
	bool		playing;
	ofxBounce	bounce;
};

#endif

class Sydfest_03 : public ofxJulapyApp
{
	
	public :
	
	Sydfest_03	();
	~Sydfest_03	();
	
	void setup		();
	void update		();
	void draw		();
	void drawDebug	();
	
	void reset		();
	
	void keyReleased	( int key );
	
	ofxCirclePacker		circlePacker;
	vector<Circle>		*circles;
	vector<CircleAnim>	circleAnims;
	
	int					frameStart;
	
	ofxBox2d				box2d;
	vector<ofxBox2dLine>	box2dLineStrips;
	
	ofxShapes			shapes;
};

#endif