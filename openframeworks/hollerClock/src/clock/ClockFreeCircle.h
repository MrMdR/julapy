/*
 *  ClockFreeCircle.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 10/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBox2dCircle.h"

class ClockFreeCircle : public ofxBox2dCircle
{
	
public:

	 ClockFreeCircle ();
	~ClockFreeCircle ();
	
	void update	();
	void draw	();
	
};