/*
 *  EventItemCloseBtn.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxMSAInteractiveObject.h"

class EventItemCloseBtn : public ofxMSAInteractiveObject
{

public:
	
	 EventItemCloseBtn() { btnId = -1; };
	~EventItemCloseBtn() {};
	
	//==================================================
	
	int				btnId;
	ofEvent<int>	btnPressEvent;
	ofImage*		image;
	
	//==================================================
	
	void setup ()
	{
		image = (ofImage*)ofxAssets :: getInstance()->getAsset( "event.closebtn" );
		
		setSize( image->width, image->height );
	}
	
	void draw()
	{
		ofFill();
		
		if( isMouseOver() )
		{
			ofSetColor( 0xFF0000 );
		}
		else
		{
			ofSetColor( 0xFFFFFF );
		}
		image->draw( x, y );
	}
	
	void onPress ( int x, int y, int button )
	{
		ofNotifyEvent( btnPressEvent, btnId, this );
	}
	
};