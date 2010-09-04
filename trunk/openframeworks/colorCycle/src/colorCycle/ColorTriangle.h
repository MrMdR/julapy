/*
 *  ColorTriangle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class ColorTriangle
{
	
public:
	
	 ColorTriangle();
	
	void init		();
	
	bool isPointInsideTriangle		( const ofPoint& p );
	ofPoint getNearestTrianglePoint	( float x, float y );
	
	void draw		();
	void drawFill	();
	void drawStroke	();
	
	ofPoint		points[ 3 ];
	ofColor		colors[ 3 ];
	int			indexs[ 3 ];
	
	float		gl_points[ 6 ];
	float		gl_colors[ 12 ];
};