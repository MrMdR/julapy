/*
 *  HeadphoneAnimation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 22/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "HeadphoneAnimation.h"

HeadphoneAnimation :: HeadphoneAnimation()
{
	load();
}

HeadphoneAnimation :: ~HeadphoneAnimation()
{
	
}

void HeadphoneAnimation :: load()
{
	string imagePath = "animations/headphone/faceoff_becks_headphones.png";
	addFrame( imagePath );
}

void HeadphoneAnimation :: draw( int x, int y, int w, int h )
{
	float dx = 0.0;
	float dy = -0.18;
	float dw = 1.45;
	float dh = 1.45;
	float ws = h / (float)height() * width();
	float hs = w / (float)width() * height();
	
	ofNoFill();
	ofSetColor( 0xFFFFFF );
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
