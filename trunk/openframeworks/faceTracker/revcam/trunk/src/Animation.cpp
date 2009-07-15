/*
 *  Animation.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 14/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Animation.h"

Animation :: Animation()
{
	reset();
}

Animation :: ~Animation()
{
	images.clear();
}

void Animation :: addFrame( string imagePath )
{
	images.push_back( ofImage() );
	images.back().loadImage( imagePath );
}

void Animation :: reset()
{
	imageIndex = 0;
}

void Animation :: draw( int x, int y, int w, int h )
{
	if( images.size() > 0 )
	{
		images.at( imageIndex ).draw( x, y, w, h );
	}
}

void Animation :: nextFrame()
{
	if( ++imageIndex > images.size() - 1 )
	{
		imageIndex = 0;
	}
}

void Animation :: prevFrame()
{
	if( --imageIndex < 0 )
	{
		imageIndex = images.size() - 1;
	}
}

void Animation :: gotoFrame( int frame )
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

int Animation :: totalFrames()
{
	return images.size();
}

int Animation :: width()
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

int Animation :: height()
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

