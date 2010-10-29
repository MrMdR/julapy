/*
 *  EventVideoItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "EventItem.h"

class EventVideoItem : public EventItem
{
	
public:
	
	 EventVideoItem() {};
	~EventVideoItem() {};
	
	//==================================================
	
	ofVideoPlayer*	eventVideo;
	bool			bVideoFinished;
	
	//==================================================
	
	void setup ()
	{
		EventItem :: setup();
		
		bg			= (ofImage*)ofxAssets :: getInstance()->getAssetByFileName( data.copy );
		
		eventVideo	= (ofVideoPlayer*)ofxAssets :: getInstance()->getAssetByFileName( data.video );
		eventVideo->setVolume( 0 );
		
		bVideoFinished = false;
		
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
		
		if( eventVideo != NULL )
		{
			eventVideo->setPosition( 0 );
			eventVideo->play();
			eventVideo->setLoopState( false );
		}
	}
	
	void hide ()
	{
		EventItem :: hide();
	
		if( eventVideo != NULL )
		{
			eventVideo->setPosition( 0 );
			eventVideo->stop();
		}
	}
	
	void update ()
	{
		EventItem :: update();
		
		if( eventVideo != NULL )
		{
			if( !bVideoFinished )
			{
				bVideoFinished = eventVideo->getIsMovieDone();
			}
		}
		
		if( bVideoFinished && bSoundFinished )
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
		
//		ofFill();
//		ofRect( x + 69, y + 72, 356, 232 );
		
		if( eventVideo != NULL )
			eventVideo->draw( x + 69, y + 72 );
		
		ofDisableAlphaBlending();
		
		EventItem :: draw();
	}
	
};