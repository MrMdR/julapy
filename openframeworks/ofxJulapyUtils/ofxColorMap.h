/*
 *  ofxColorMap.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 27/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_COLOR_MAP_H_
#define _OFX_COLOR_MAP_H_

#include "ofMain.h"

class ofxColorMap
{
	
public :
	
	 ofxColorMap();
	~ofxColorMap();
	
	void setup		( ofImage *image );
	
	int  getColor	( const ofPoint& p );
	int  getColor	( float x, float y );
	
private :
	
	ofImage* image;
	
};

#endif