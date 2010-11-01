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

#include "SoundManager.h"
#include "EventDataItem.h"
#include "Btn.h"

class TimelineMarker : public ofxSprite
{
	
public:
	
	TimelineMarker()
	{
		setup();
	}
	
	~TimelineMarker() 
	{
		//
	};
	
	//==================================================
	
	EventDataItem	data;
	
	ofImage*		markerRainUp;
	ofImage*		markerRainDown;
	ofImage*		markerTempUp;
	ofImage*		markerTempDown;
	ofPoint			offset;
	
	Btn				btn;
	
	int				tabType;
	
	ofEvent<int>	markerPressEvent;
	
	//==================================================

	void populate ( EventDataItem data )
	{
		this->data = data;
	}
	
	void setup()
	{
		markerRainUp	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar_marker_rain_a" );
		markerRainDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar_marker_rain_b" );
		markerTempUp	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar_marker_temp_a" );
		markerTempDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bar_marker_temp_b" );
		
		offset.x = -16;
		offset.y = -9;
		
		btn.setID( data.id );
		btn.setSize( 32, 53 );
		ofAddListener( btn.btnPressEvent, this, &TimelineMarker :: btnPressed );
		
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
		
		btn.setPos( this->x, this->y );
	}
	
	void update ()
	{
		//
	}
	
	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();
		
		bool bMouseDown;
		bMouseDown = (  btn.isMouseOver() || btn.isMouseDown() );
		
		if( tabType == TIMELINE_TAB_RAIN )
		{
			if(  bMouseDown )
			{
				markerRainDown->draw( x, y );
			}
			else
			{
				markerRainUp->draw( x, y );
			}

		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			if(  bMouseDown )
			{
				markerTempDown->draw( x, y );
			}
			else
			{
				markerTempUp->draw( x, y );	
			}

		}
		
//		drawBounds();
		
		ofDisableAlphaBlending();
	}
	
	void drawBounds ()
	{
		ofNoFill();
		ofSetColor( 0x00FF00 );
		ofRect( btn.x, btn.y, btn.width, btn.height );
	}
	
	//==================================================
	
	void btnPressed ( int& btnId )
	{
		ofNotifyEvent( markerPressEvent, data.id, this );
		
		SoundManager* sound;
		sound = SoundManager :: getInstance();
		sound->playClick();
	}
};