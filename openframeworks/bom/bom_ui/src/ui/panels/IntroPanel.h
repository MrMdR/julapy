/*
 *  IntroPanel.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 22/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

class IntroPanel : public ofxSprite
{
	
public:
	
	IntroPanel()
	{
		setup();
	}
	
	~IntroPanel() {};
	
	//==================================================
	
	ofImage* image;
	
	//==================================================
	
	void setup ()
	{
		x = 1;
		y = 0;
		
		image = (ofImage*)ofxAssets :: getInstance()->getAsset( "intro_panel" );
	}
	
	void update ()
	{
		
	}
	
	void draw ()
	{
		ofEnableAlphaBlending();
		
		image->draw( x, y );
		
		ofDisableAlphaBlending();
	}
};