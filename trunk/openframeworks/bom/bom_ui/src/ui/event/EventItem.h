/*
 *  EventItem.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "BasePanel.h"
#include "ofxSprite.h"
#include "EventDataItem.h"
#include "EventItemCloseBtn.h"

class EventItem : public BasePanel
{

public:
	
	EventItem()
	{
		rect.width	= 400;
		rect.height	= 400;
		rect.x = x	= (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
		rect.y = y	= (int)( ( ofGetHeight() - rect.height ) * 0.5 );
		
		closeBtn.setup();
		closeBtn.setPos( rect.x + rect.width - 32, rect.y + 10 );
		closeBtn.enableMouseEvents();
		
		sound = NULL;
		
		bSoundFinished	= false;
		bFinished		= false;
		
		ofAddListener( closeBtn.btnPressEvent, this, &EventItem :: closeBtnPressed );
	};
	
	~EventItem()
	{
		closeBtn.disableAllEvents();
		
		ofRemoveListener( closeBtn.btnPressEvent, this, &EventItem :: closeBtnPressed );
	};
	
	//==================================================
	
	ofRectangle			rect;
	EventDataItem		data;
	ofImage*			bg;
	ofSoundPlayer*		sound;
	bool				bSoundFinished;
	bool				bFinished;
	
	EventItemCloseBtn	closeBtn;
	ofEvent<int>		closeEvent;
	ofEvent<bool>		finishedEvent;
	
	//==================================================
	
	void populate ( EventDataItem data )
	{
		this->data = data;
	}
	
	virtual void setup ()
	{
		sound = ofxAssets :: getInstance()->getSoundByFileName( data.sound );
	}
	
	virtual void show ()
	{
		visible = true;
		
		closeBtn.enabled = true;
		
		if( sound != NULL )
		{
			sound->play();
			sound->setPosition( 0 );
			sound->setPaused( false );
		}
	}
	
	virtual void hide ()
	{
		visible = false;
		
		closeBtn.enabled = false;
		
		if( sound != NULL )
		{
			sound->setPaused( true );
		}
	}
	
	virtual void update ()
	{
		BasePanel :: update();
		
		if( sound != NULL )
		{
			if( !bSoundFinished )
			{
				bSoundFinished = ( sound->getPosition() == 1.0 );
			}
		}
	}
	
	virtual void draw ()
	{
		ofEnableAlphaBlending();
		ofSetColor( 255, 255, 255, 255 * alpha );
		
		closeBtn.draw();
		
		ofDisableAlphaBlending();
	}
	
	void closeBtnPressed ( int & btnId )
	{
		ofNotifyEvent( closeEvent, data.id, this );
	}
	
};