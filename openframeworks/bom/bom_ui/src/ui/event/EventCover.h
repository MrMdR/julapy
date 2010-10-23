/*
 *  EventCover.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 23/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

class EventCover : public ofxSprite
{

public:
	
	EventCover()
	{
		//
	}
	
	~EventCover()
	{
		//
	}
	
	void draw ()
	{
		ofEnableAlphaBlending();
		
		ofFill();
		ofSetColor( 0, 0, 0, 0.7 * 255 );
		ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
		
		ofDisableAlphaBlending();
	}
};