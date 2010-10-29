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
		EventItem :: setup();
		
		bg = (ofImage*)ofxAssets :: getInstance()->getAssetByFileName( data.copy );
		
		if( bg != NULL )
		{
			rect.width	= bg->width;
			rect.height	= bg->height;
			rect.x		= (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
			rect.y		= (int)( ( ofGetHeight() - rect.height ) * 0.5 - 70 );
			
			x = rect.x;
			y = rect.y;
			
			closeBtn.setPos( x + 416, y + 50 );
		}
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
		
		if( bSoundFinished )
		{
			if( !bFinished )
			{
				bFinished = true;
				
				ofNotifyEvent( finishedEvent, bSoundFinished, this );
			}
		}
	}
	
	void draw ()
	{
		if( !visible )
			return;
		
		ofSetColor( 255, 255, 255, 255 * alpha );
		ofEnableAlphaBlending();
		
		if( bg != NULL )
		{
			bg->draw( x, y );
		}
		
		ofDisableAlphaBlending();
		
		EventItem :: draw();
	}
	
};