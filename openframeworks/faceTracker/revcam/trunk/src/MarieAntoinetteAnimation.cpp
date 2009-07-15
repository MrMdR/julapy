/*
 *  MarieAntoinetteAnimation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MarieAntoinetteAnimation.h"

MarieAntoinetteAnimation :: MarieAntoinetteAnimation()
{
	load();
}

MarieAntoinetteAnimation :: ~MarieAntoinetteAnimation()
{
	
}

void MarieAntoinetteAnimation :: load()
{
	for( int i=0; i<84; i++ )
	{
		string imagePath = "animations/marieantoinette/marieantoinette_win" + ofToString( i ) + ".png";
		addFrame( imagePath );
	}
}

void MarieAntoinetteAnimation :: draw( int x, int y, int w, int h )
{
	float dx = -0.34;
	float dy = -0.9;
	float dw = 3.3;
	float dh = 3.3;
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