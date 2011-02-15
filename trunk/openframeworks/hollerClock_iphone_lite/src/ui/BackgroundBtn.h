/*
 *  BackgroundBtn.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 11/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlashMovieClip.h"

class BackgroundBtn
{

public:
	
	 BackgroundBtn( ofxFlashMovieClip* asset );
	~BackgroundBtn();

	void update		();
	bool isPressed	();
	
	ofxFlashMovieClip*	asset;
	
	bool bPressed;
	bool bPressedChanged;
};