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
	
	ofImage*	rainPlay;
	ofImage*	rainPause;
	ofImage*	tempPlay;
	ofImage*	tempPause;
	
	Btn*		btn;
	int			btnId;
	int			tabType;
	bool		bPlay;
	
	//==================================================
	
	void setup ()
	{
		x = 14;
		y = 608;
		
		rainPlay	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_rain_0" );
		rainPause	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_rain_1" );
		tempPlay	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_temp_0" );
		tempPause	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_playBtn_temp_1" );
		
		btn		= new Btn();
		btn->setPos( x, y );
		btn->setSize( rainPlay->width, rainPlay->height );
		
		ofAddListener( btn->btnOverEvent,	this, &TimelinePlayBtn :: btnOver );
		ofAddListener( btn->btnOutEvent,	this, &TimelinePlayBtn :: btnOut );
		ofAddListener( btn->btnPressEvent,	this, &TimelinePlayBtn :: btnPressed );
		
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

		if( tabType == TIMELINE_TAB_RAIN )
		{
			if( bPlay )
			{
				rainPause->draw( x, y );
			}
			else 
			{
				rainPlay->draw( x, y );
			}
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			if( bPlay )
			{
				tempPause->draw( x, y );
			}
			else 
			{
				tempPlay->draw( x, y );
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
	}
	
	ofEvent<int> btnPressEvent;
};