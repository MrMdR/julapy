/*
 *  MarianneAnimation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MarianneAnimation.h"

MarianneAnimation :: MarianneAnimation()
{
	load();
}

MarianneAnimation :: ~MarianneAnimation()
{
	
}

void MarianneAnimation :: load()
{
	for( int i=0; i<102; i++ )
	{
		string imagePath = "animations/marianne/marianne_win" + ofToString( i ) + ".png";
		addFrame( imagePath );
	}
}

void MarianneAnimation :: draw( int x, int y, int w, int h )
{
	float dx = -1.28;
	float dy = 0.7;
	float dw = 4.8;
	float dh = 4.8;
	float ws = h / (float)height() * width();
	float hs = w / (float)width() * height();
	
	ofNoFill();
	ofSetColor(0xFFFFFF);
	ofEnableAlphaBlending();
	
	if( images.size() > 0 )
	{
		images.at( imageIndex ).draw
		(
			x - w * ( dw - 1 ) * 0.5 + dx * w,
			y - h * ( dh - 1 ) * 0.5 + dy * h,
			w * dw,
			hs * dh
		 );
	}
	
	ofDisableAlphaBlending();
}