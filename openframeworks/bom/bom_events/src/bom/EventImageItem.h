/*
 *  EventImageItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "EventItem.h"

class EventImageItem : public EventItem
{
	
public:
	
	 EventImageItem() {};
	~EventImageItem() {};
	
	//==================================================
	
	ofImage*		eventImage;
	
	//==================================================
	
	void setup ()
	{
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "event.bg.image" );
		eventImage	= (ofImage*)ofxAssets :: getInstance()->getAsset( "event.cyclone" );
	}
	
	void show ()
	{
		EventItem :: show();
		
		//
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
		bg->draw( x, y );
		eventImage->draw( x, y );
		
		EventItem :: draw();
	}
	
};