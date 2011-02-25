/*
 *  Btn.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 26/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxFlash.h"

class Btn
{
	
public:
	
	Btn( ofxFlashMovieClip* asset )
	{
		this->asset = asset;
		asset->mouseEnabled( true );
		
		bPressed		= false;
		bPressedChanged	= false;
	}
	
	~Btn()
	{
		//
	}
	
	ofxFlashMovieClip* asset;
	bool bPressed;
	bool bPressedChanged;
	
	void update ()
	{
		bPressedChanged	= bPressed != asset->mouseDown();
		bPressed		= asset->mouseDown();
	}
	
	bool isPressed ()
	{
		return bPressed && bPressedChanged;
	}	
	
};