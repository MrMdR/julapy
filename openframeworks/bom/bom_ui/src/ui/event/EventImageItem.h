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
		EventItem :: setup();
		
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( data.copy );
		eventImage	= (ofImage*)ofxAssets :: getInstance()->getAssetByFileName( data.image );
		
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
		
		if( eventImage != NULL )
			eventImage->draw( x + 69, y + 72 );
		
		ofDisableAlphaBlending();
		
		EventItem :: draw();
	}
	
};