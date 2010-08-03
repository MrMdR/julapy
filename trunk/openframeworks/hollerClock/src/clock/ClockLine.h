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
	
	void setTexture	( ofTexture* tex );
	void draw		();
	
	ofTexture*		tex;
	float			angle;
	float			scale;
};

#endif