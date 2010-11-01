/*
 *  EventElninoItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 27/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "EventItem.h"

class EventElninoItem : public EventItem
{
	
public:
	
	 EventElninoItem() {};
	~EventElninoItem() {};
	
	//==================================================
	
	ofVideoPlayer*	eventVideo;
	
	//==================================================
	
	void setup ()
	{
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "elnino_copy" );
		eventVideo	= (ofVideoPlayer*)ofxAssets :: getInstance()->getAsset( "elnino_video" );
		sound		= ofxAssets :: getInstance()->getSound( "elnino_sound" );
		
		if( bg != NULL )
		{
			rect.width	= bg->width;
			rect.height	= bg->height;
			rect.x		= (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
			rect.y		= (int)( ( ofGetHeight() - rect.height ) * 0.5 - 80 );
			
			x = rect.x;
			y = rect.y;
			
			closeBtn.setPos( x + 572, y + 50 );
		}
	}
	
	void show ()
	{
		EventItem :: show();
		
		if( eventVideo != NULL )
		{
			eventVideo->setPosition( 0 );
			eventVideo->play();
		}
	}
	
	void hide ()
	{
		EventItem :: hide();
		
		if( eventVideo != NULL )
		{
			eventVideo->stop();
		}
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
			bg->draw( x, y );
		
		ofFill();
		ofRect( x + 83, y + 83, 486, 233 );
		
		if( eventVideo != NULL )
			eventVideo->draw( x + 83, y + 83 );
		
		ofDisableAlphaBlending();
		
		EventItem :: draw();
	}
	
};