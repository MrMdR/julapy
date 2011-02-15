/*
 *  BackgroundBtn.cpp
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 11/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BackgroundBtn.h"

BackgroundBtn :: BackgroundBtn( ofxFlashMovieClip* asset )
{
	this->asset = asset;
	asset->mouseEnabled( true );
	asset->alpha( 0 );
	
	bPressed		= false;
	bPressedChanged	= false;
}

BackgroundBtn :: ~BackgroundBtn()
{
	//
}

void BackgroundBtn :: update ()
{
	bPressedChanged	= bPressed != asset->mouseDown();
	bPressed		= asset->mouseDown();
	
//	if( bPressed )
//	{
//		asset->alpha( 1.0 );
//	}
//	else
//	{
//		asset->alpha( 0.5 );
//	}
}

bool BackgroundBtn :: isPressed ()
{
	return bPressed && bPressedChanged;
}