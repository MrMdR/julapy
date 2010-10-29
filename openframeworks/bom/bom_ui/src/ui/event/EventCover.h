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
#include "BasePanel.h"

class EventCover : public BasePanel
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
	
	void update()
	{
		BasePanel :: update();
	}
	
	void draw ()
	{
		ofEnableAlphaBlending();
		
		ofFill();
		ofSetColor( 0, 0, 0, 0.7 * 255 * alpha );
		ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
		
		ofDisableAlphaBlending();
	}
};