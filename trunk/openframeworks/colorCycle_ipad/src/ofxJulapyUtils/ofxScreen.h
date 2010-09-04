/*
 *  ofxScreen.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class ofxScreen
{

public:
	
	ofxScreen()
	{
		set( 0, 0 );
	}
	
	ofxScreen( int w, int h )
	{
		set( w, h );
	}
	
	~ofxScreen ()
	{
		//
	}
	
	//-----------------------------------
	
	int screenWidth;
	int screenHeight;
	int screenMax;
	int screenMin;
	int screenArea;
	float screenDiagonal;

	//-----------------------------------
	
	void set ( int w, int h )
	{
		screenWidth		= w;
		screenHeight	= h;
		screenMax		= MAX( w, h );
		screenMin		= MIN( w, h );
		screenArea		= w * h;
		screenDiagonal	= sqrt( w * w + h * h );
	}	
	
};