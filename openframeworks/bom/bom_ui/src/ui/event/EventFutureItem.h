/*
 *  EventFutureItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 27/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "EventItem.h"

class EventFutureItem : public EventItem
{
	
public:
	
	 EventFutureItem() {};
	~EventFutureItem() {};
	
	//==================================================
	
	ofImage*	futureImage;
	
	//==================================================
	
	void setup ()
	{
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "2_4_V" );
		futureImage	= (ofImage*)ofxAssets :: getInstance()->getAsset( "image_sample_356x232" );
		sound		= ofxAssets :: getInstance()->getSound( "sound_2_2" );
		
		if( bg != NULL )
		{
			rect.width	= bg->width;
			rect.height	= bg->height;
			rect.x		= (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
			rect.y		= (int)( ( ofGetHeight() - rect.height ) * 0.5 - 80 );
			
			x = rect.x;
			y = rect.y;
			
			closeBtn.setPos( x + 416, y + 50 );
		}
	}
	
	void show ()
	{
		EventItem :: show();
	}
	
	void hide ()
	{
		EventItem :: hide();
	}
	
	void update ()
	{
		EventItem :: update();
	}
	
	void draw ()
	{
		if( !visible )
			return;
		
		ofSetColor( 0xFFFFFF );
		ofEnableAlphaBlending();
		
		if( bg != NULL )
			bg->draw( x, y );
		
		ofFill();
		ofRect( x + 69, y + 72, 356, 232 );
		
		if( futureImage != NULL )
			futureImage->draw( x + 69, y + 72 );
		
		ofDisableAlphaBlending();
		
		EventItem :: draw();
	}
	
};