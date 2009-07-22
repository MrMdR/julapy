/*
 *  Animation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 14/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFrameAnimation.h"

ofxFrameAnimation :: ofxFrameAnimation()
{
	reset();
}

ofxFrameAnimation :: ~ofxFrameAnimation()
{
	images.clear();
}

void ofxFrameAnimation :: addFrame( string imagePath )
{
	images.push_back( ofImage() );
	images.back().loadImage( imagePath );
}

void ofxFrameAnimation :: reset()
{
	imageIndex = 0;
}

void ofxFrameAnimation :: draw( int x, int y, int w, int h )
{
	if( images.size() > 0 )
	{
		images.at( imageIndex ).draw( x, y, w, h );
	}
}

void ofxFrameAnimation :: nextFrame()
{
	if( ++imageIndex > images.size() - 1 )
	{
		imageIndex = 0;
	}
}

void ofxFrameAnimation :: prevFrame()
{
	if( --imageIndex < 0 )
	{
		imageIndex = images.size() - 1;
	}
}

void ofxFrameAnimation :: gotoFrame( int frame )
{
	if( frame < 0 )
	{
		frame = 0;
	}
	
	if( frame > images.size() - 1 )
	{
		frame = images.size() - 1;
	}
	
	imageIndex = frame;
}

int ofxFrameAnimation :: totalFrames()
{
	return images.size();
}

int ofxFrameAnimation :: width()
{
	if( images.size() > 0 )
	{
		return images.at( imageIndex ).width;
	}
	else
	{
		return 0;
	}
}

int ofxFrameAnimation :: height()
{
	if( images.size() > 0 )
	{
		return images.at( imageIndex ).height;
	}
	else
	{
		return 0;
	}
}

