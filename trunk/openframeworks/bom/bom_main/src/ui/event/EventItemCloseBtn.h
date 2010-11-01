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
	ofImage*		imageUp;
	ofImage*		imageDown;
	
	//==================================================
	
	void setup ()
	{
		imageUp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "event_close_icon_a" );
		imageDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "event_close_icon_b" );
		
		setSize( imageUp->width, imageUp->height );
	}
	
	void draw()
	{
		ofFill();
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();
		
		bool bMouseDown;
		bMouseDown = (  isMouseOver() || isMouseDown() );
		
		if( bMouseDown )
		{
			imageDown->draw( x, y );
		}
		else
		{
			imageUp->draw( x, y );
		}
		
		ofDisableAlphaBlending();
	}
	
	void onPress ( int x, int y, int button )
	{
		ofNotifyEvent( btnPressEvent, btnId, this );
	}
	
};