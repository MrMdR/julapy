/*
 *  SarkAnimation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SarkoAnimation.h"

SarkoAnimation :: SarkoAnimation()
{
	load();
}

SarkoAnimation :: ~SarkoAnimation()
{
	
}

void SarkoAnimation :: load()
{
	for( int i=0; i<96; i++ )
	{
		string imagePath = "animations/sarko/sarko_win" + ofToString( i ) + ".png";
		addFrame( imagePath );
	}
}

void SarkoAnimation :: draw( int x, int y, int w, int h )
{
	float dx = -0.95;
	float dy = 0.2;
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