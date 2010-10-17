/*
 *  EventTextItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "EventItem.h"

class EventTextItem : public EventItem
{
	
public:
	
	 EventTextItem() {};
	~EventTextItem() {};
	
	//==================================================
	
	
	
	//==================================================
	
	void setup ()
	{
		bg = (ofImage*)ofxAssets :: getInstance()->getAsset( "event.bg.text" );
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
		
		EventItem :: draw();
	}
	
};