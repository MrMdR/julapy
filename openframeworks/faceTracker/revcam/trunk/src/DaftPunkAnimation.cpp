/*
 *  DaftPunkAnimation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "DaftPunkAnimation.h"

DaftPunkAnimation :: DaftPunkAnimation()
{
	load();
}

DaftPunkAnimation :: ~DaftPunkAnimation()
{
	
}

void DaftPunkAnimation :: load()
{
	for( int i=0; i<94; i++ )
	{
		string imagePath = "animations/daftpunk/daftpunk_win" + ofToString( i ) + ".png";
		addFrame( imagePath );
	}
}

void DaftPunkAnimation :: draw( int x, int y, int w, int h )
{
	float dx = 0;
	float dy = -0.4;
	float dw = 1.9;
	float dh = 2.0;
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