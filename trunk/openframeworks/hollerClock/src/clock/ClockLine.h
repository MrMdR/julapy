/*
 *  ClockLine.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 3/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLOCK_LINE_H
#define CLOCK_LINE_H

#include "ofMain.h"

class ClockLine
{

public :
	
	 ClockLine	();
	~ClockLine	();
	
	void setTexture		( ofTexture* tex );
	void setup			();
	
	void draw			( float w = 100, float h = 1 );
	void drawTexture	( float w = 100, float h = 1 );
	void drawLine		( float w = 100, float h = 1 );
	
	
	ofTexture*		tex;
	float			angle;
	float			scale;
	float			length;
	ofRectangle		size;
	
	float*			pointsPos;
	float*			pointsVel;
	int				pointsNum;
	
	float			linePoints[ 4 ];
	float			lineColors[ 8 ];
	float			rectPoints[ 8 ];
};

#endif