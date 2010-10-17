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
#include "ofxSprite.h"
#include "EventDataItem.h"
#include "EventItemCloseBtn.h"

class EventItem : public ofxSprite
{

public:
	
	EventItem()
	{
		itemId = -1;
		
		rect.width	= 400;
		rect.height	= 400;
		rect.x = x	= (int)( ( ofGetWidth()  - rect.width  ) * 0.5 );
		rect.y = y	= (int)( ( ofGetHeight() - rect.height ) * 0.5 );
		
		closeBtn.setup();
		closeBtn.setPos( rect.x + rect.width - closeBtn.width, rect.y );
		closeBtn.enableMouseEvents();
		
		ofAddListener( closeBtn.btnPressEvent, this, &EventItem :: closeBtnPressed );
	};
	
	~EventItem()
	{
		//
	};
	
	//==================================================
	
	int					itemId;
	ofRectangle			rect;
	EventDataItem		data;
	ofImage				bg;
	
	EventItemCloseBtn	closeBtn;
	ofEvent<int>		closeEvent;
	
	//==================================================
	
	void populate ( EventDataItem data, int itemId )
	{
		this->data		= data;
		this->itemId	= itemId;
	}
	
	virtual void show ()
	{
		visible = true;
		
		closeBtn.enabled = true;
	}
	
	virtual void hide ()
	{
		visible = false;
		
		closeBtn.enabled = false;
	}
	
	virtual void setup ()
	{
		//
	}
	
	virtual void update ()
	{
		//
	}
	
	virtual void draw ()
	{
		closeBtn.draw();
	}
	
	void closeBtnPressed ( int & btnId )
	{
		ofNotifyEvent( closeEvent, itemId, this );
	}
	
};