/*
 *  Sydfest_05.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 29/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SYDFEST_05_H_
#define _SYDFEST_05_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxCirclePacker.h"
#include "ofxSimpleGuiToo.h"
#include "ofxColorUtils.h"
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

class Sydfest_05 : public ofxJulapyApp
{
	
	public :
	
	 Sydfest_05	();
	~Sydfest_05	();
	
	void setup		();
	void update		();
	void draw		();
	void drawDebug	();
	
	void drawCirclesLayer	( vector<Circle> *circles, int circlesAlpha );
	
	void reset		();
	
	void keyReleased	( int key );
	
	ofxCirclePacker		circlePacker01;
	ofxCirclePacker		circlePacker02;
	ofxCirclePacker		circlePacker03;
	ofxCirclePacker		circlePacker04;
	
	vector<Circle>		*circles01;
	vector<Circle>		*circles02;
	vector<Circle>		*circles03;
	vector<Circle>		*circles04;
	
	int					circlesAlpha01;
	int					circlesAlpha02;
	int					circlesAlpha03;
	int					circlesAlpha04;
	
	vector<CircleAnim>	circleAnims;
	
	int					frameStart;
	
	ofxSimpleGuiToo		gui;
};

#endif