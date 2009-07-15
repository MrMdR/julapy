/*
 *  CarlaAnimation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "CarlaAnimation.h"

CarlaAnimation :: CarlaAnimation()
{
	load();
}

CarlaAnimation :: ~CarlaAnimation()
{

}

void CarlaAnimation :: load()
{
	for( int i=0; i<72; i++ )
	{
		string imagePath = "animations/carla/carla_win" + ofToString( i ) + ".png";
		addFrame( imagePath );
	}
}

void CarlaAnimation :: draw( int x, int y, int w, int h )
{
	float dx = 0.03;
	float dy = -0.25;
	float dw = 2.15;
	float dh = 2.15;
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