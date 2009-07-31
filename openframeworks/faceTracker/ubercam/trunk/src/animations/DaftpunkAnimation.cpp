/*
 *  DaftpunkAnimation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 22/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "DaftpunkAnimation.h"

DaftpunkAnimation :: DaftpunkAnimation()
{
	load();
}

DaftpunkAnimation :: ~DaftpunkAnimation()
{
	
}

void DaftpunkAnimation :: load()
{
	string imagePath = "animations/faceoff_becks_0_05.png";
	addFrame( imagePath );
}

void DaftpunkAnimation :: draw( int x, int y, int w, int h )
{
	float dx = 0.0;
	float dy = -0.07;
	float dw = 1.8;
	float dh = 1.8;
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
