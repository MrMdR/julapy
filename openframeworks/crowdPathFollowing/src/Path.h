/*
 *  Path.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 28/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PATH
#define PATH

#include "ofMain.h"
#include "ofxVectorMath.h"

class Path
{
	
public :
	
	vector<ofxVec2f> points;
	float radius;
	
	void init();
	void addPoint( float x, float y );
	void display();
};

#endif