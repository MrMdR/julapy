/*
 *  TestBtn.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxMSAInteractiveObject.h"

class TestBtn : public ofxMSAInteractiveObject
{

public:
	
	 TestBtn() { btnId = -1; };
	~TestBtn() {};
	
	int btnId;
	
	void populate ( int btnId )
	{
		this->btnId = btnId;
	}
	
	void draw()
	{
		ofFill();
		ofSetColor( 0xFF0000 );
		ofRect( x, y, width, height );
	}
	
	void onPress ( int x, int y, int button )
	{
		ofNotifyEvent( btnPressEvent, btnId, this );
	}
	
	ofEvent<int> btnPressEvent;
};