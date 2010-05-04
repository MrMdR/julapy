/*
 *  Backdrop.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 7/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HOLLER_PONG_BACKDROP
#define HOLLER_PONG_BACKDROP

#include "ofMain.h"
#include "PongConfig.h"
#include "ofxVectorMath.h"

#define STAR_LAYERS_TOTAL	3

struct Star
{
	ofxVec2f	p;
	int			c;
};

class Backdrop 
{

public :
	
	void init		();
	void update		();
	void draw		();
	
	void scatter	( vector<Star>& stars, int noStars, float min = 0, float max = 1 );
	void setBallPos	( float x, float y );
	
	void drawDivider	();
	void drawStarLayers	();
	void drawStars		( vector<Star>& stars, ofImage& image );

	vector<int>			colors;
	
	int					starsTotal;
	vector<Star>		stars		[ STAR_LAYERS_TOTAL ];
	ofImage				starImages	[ STAR_LAYERS_TOTAL ];
	float				starEase	[ STAR_LAYERS_TOTAL ];
	ofPoint				starOffset	[ STAR_LAYERS_TOTAL ];
	
};

#endif