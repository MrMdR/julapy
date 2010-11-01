/*
 *  PlayBtn.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 21/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "Model.h"

class TimelinePlayBtn : public ofxSprite
{

public:
	
	TimelinePlayBtn( int btnId = -1 )
	{
		setID( btnId );
		
		setup();
	}
	
	~TimelinePlayBtn() {};
	
	//==================================================
	
	ofImage*	rainPlayUp;
	ofImage*	rainPlayDown;
	ofImage*	rainPauseUp;
	ofImage*	rainPauseDown;
	ofImage*	tempPlayUp;
	ofImage*	tempPlayDown;
	ofImage*	tempPauseUp;
	ofImage*	tempPauseDown;
	
	Btn			btn;
	int			btnId;
	int			tabType;
	bool		bPlay;
	
	ofEvent<int> btnPressEvent;
	
	//==================================================
	
	void setup ()
	{
		x = 14;
		y = 608;
		
		rainPlayUp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_rain_0a" );
		rainPlayDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_rain_0b" );
		rainPauseUp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_rain_1a" );
		rainPauseDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_rain_1b" );
		tempPlayUp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_temp_0a" );
		tempPlayDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_temp_0b" );
		tempPauseUp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_temp_1a" );
		tempPauseDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_temp_1b" );
		
		btn.setPos( x, y );
		btn.setSize( 77, 77 );
		
		ofAddListener( btn.btnOverEvent,	this, &TimelinePlayBtn :: btnOver );
		ofAddListener( btn.btnOutEvent,		this, &TimelinePlayBtn :: btnOut );
		ofAddListener( btn.btnPressEvent,	this, &TimelinePlayBtn :: btnPressed );
		
		//-- model.
		
		Model* model;
		model = Model :: getInstance();
		
		tabType = model->getTabType();
		bPlay	= model->getTimelinePlay();
		
		ofAddListener( model->tabTypeChangeEvent,		this, &TimelinePlayBtn :: tabTypeChanged );
		ofAddListener( model->timelinePlayChangeEvent,	this, &TimelinePlayBtn :: timelinePlayChanged );
	}
	
	void tabTypeChanged ( int & tabType )
	{
		this->tabType = tabType;
	}
	
	void timelinePlayChanged ( bool & bPlay )
	{
		this->bPlay = bPlay;
	}
	
	void setID ( int btnId )
	{
		this->btnId = btnId;
	}
	
	int getID ()
	{
		return btnId;
	}
	
	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();

		bool bMouseDown;
		bMouseDown = (  btn.isMouseOver() || btn.isMouseDown() );
		
		if( tabType == TIMELINE_TAB_RAIN )
		{
			if( bPlay )
			{
				if(  bMouseDown )
				{
					rainPauseDown->draw( x, y );
				}
				else
				{
					rainPauseUp->draw( x, y );
				}
			}
			else 
			{
				if( bMouseDown )
				{
					rainPlayDown->draw( x, y );
				}
				else
				{
					rainPlayUp->draw( x, y );
				}
			}
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			if( bPlay )
			{
				if(  bMouseDown )
				{
					tempPauseDown->draw( x, y );
				}
				else
				{
					tempPauseUp->draw( x, y );
				}

			}
			else 
			{
				if(  bMouseDown )
				{
					tempPlayDown->draw( x, y );
				}
				else
				{
					tempPlayUp->draw( x, y );
				}
			}
		}
		
		ofDisableAlphaBlending();
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
		Model* model;
		model = Model :: getInstance();
		model->toggleTimelinePlay();
		
		//---
		
		SoundManager* sound;
		sound = SoundManager :: getInstance();
		sound->playClick();
	}
};