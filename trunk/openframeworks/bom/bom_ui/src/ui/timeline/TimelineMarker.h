/*
 *  TimelineMarker.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 21/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "EventDataItem.h"

#include "Btn.h"

class TimelineMarker : public ofxSprite
{
	
public:
	
	TimelineMarker( int btnId = -1 )
	{
		setID( btnId );
		
		setup();
	}
	
	~TimelineMarker () {};
	
	//==================================================
	
	EventDataItem	data;
	
	ofImage*		markerRain;
	ofImage*		markerTemp;
	ofPoint			offset;
	
	Btn*			btn;
	int				btnId;
	
	int				tabType;
	
	ofEvent<int>	markerPressEvent;
	
	//==================================================

	void populate ( EventDataItem& data )
	{
		this->data = data;
	}
	
	void setup()
	{
		markerRain = (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar_marker_rain" );
		markerTemp = (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar_marker_temp" );
		
		btn = new Btn();
		btn->setSize( 32, 53 );
		
		offset.x = -16;
		offset.y = -9;
		
		ofAddListener( btn->btnOverEvent,	this, &TimelineMarker :: btnOver );
		ofAddListener( btn->btnOutEvent,	this, &TimelineMarker :: btnOut );
		ofAddListener( btn->btnPressEvent,	this, &TimelineMarker :: btnPressed );
		
		//-- model.
		
		Model* model;
		model = Model :: getInstance();
		
		tabType = model->getTabType();
		
		ofAddListener( model->tabTypeChangeEvent, this, &TimelineMarker :: tabTypeChanged );
	}
	
	void tabTypeChanged ( int& tabType )
	{
		this->tabType = tabType;
	}
	
	void setPos( int x, int y )
	{
		this->x = x + offset.x;
		this->y = y + offset.y;
		
		btn->setPos( this->x, this->y );
	}
	
	void setID ( int btnId )
	{
		this->btnId = btnId;
	}
	
	int getID ()
	{
		return btnId;
	}
	
	void update ()
	{
		//
	}
	
	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();
		
		if( tabType == TIMELINE_TAB_RAIN )
		{
			markerRain->draw( x, y );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			markerTemp->draw( x, y );
		}
		
		drawBounds();
		
		ofDisableAlphaBlending();
	}
	
	void drawBounds ()
	{
		ofNoFill();
		ofSetColor( 0x00FF00 );
		ofRect( btn->x, btn->y, btn->width, btn->height );
	}
	
	//==================================================
	
	void btnOver ( int & btnId )
	{
		//
	}
	
	void btnOut ( int & btnId )
	{
		//
	}
	
	void btnPressed ( int & btnId )
	{
		ofNotifyEvent( markerPressEvent, this->data.id, this );
	}
};