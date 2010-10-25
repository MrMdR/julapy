/*
 *  TimelineTab.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 20/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "Model.h"
#include "TimelineTabBtn.h"
#include "Btn.h"

class TimelineTab : public ofxSprite
{

public:
	
	TimelineTab()
	{
		setup();
	};
	
	~TimelineTab() {};
	
	//==================================================

	ofImage*			bg;
	ofImage*			lineRain;
	ofImage*			lineTemp;
	ofImage*			tabRain;
	ofImage*			tabTemp;
	ofImage*			futureBtnBm;
	int					tabType;
	
	TimelineTabBtn		rainBtn;
	TimelineTabBtn		tempBtn;
	Btn					futureBtn;
	
	ofEvent<int>		changeEvent;
	
	//==================================================
	
	void setup ()
	{
		x		= 0;
		y		= 521;
		
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bg" );
		tabRain		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_rain" );
		tabTemp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_temp" );
		lineRain	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_line_rain" );
		lineTemp	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_line_temp" );
		futureBtnBm	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_futureBtn" );

		rainBtn.setup();
		rainBtn.setIcon( (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_rain_icon" ) );
		rainBtn.setBlack( (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_rain_black" ) );
		rainBtn.setPos( 0, 534 );
		rainBtn.setSize( 188, 42 );
		rainBtn.setID( TIMELINE_TAB_RAIN );
		rainBtn.show();
		
		tempBtn.setup();
		tempBtn.setIcon( (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_temp_icon" ) );
		tempBtn.setBlack( (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_temp_black" ) );
		tempBtn.setPos( 187, 534 );
		tempBtn.setSize( 232, 42 );
		tempBtn.setID( TIMELINE_TAB_TEMP );
		tempBtn.hide();
		
		futureBtn.setPos( x + 1224, y + 107 );
		if( futureBtnBm != NULL )
			futureBtn.setSize( futureBtnBm->width, futureBtnBm->height );
		
		addChild( &rainBtn );
		addChild( &tempBtn );
		
		ofAddListener( rainBtn.btnPressEvent,	this, &TimelineTab :: btnPressed );
		ofAddListener( tempBtn.btnPressEvent,	this, &TimelineTab :: btnPressed );
		ofAddListener( futureBtn.btnPressEvent, this, &TimelineTab :: futureBtnPressed );
		
		//-- model.
		
		Model* model;
		model = Model :: getInstance();
		
		tabType = model->getTabType();
		
		ofAddListener( model->tabTypeChangeEvent, this, &TimelineTab :: tadChanged );
	}
	
	void tadChanged ( int & tabType )
	{
		this->tabType = tabType;
	}
	
	void update ()
	{
		if( tabType == TIMELINE_TAB_RAIN )
		{
			rainBtn.show();
			tempBtn.hide();
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			rainBtn.hide();
			tempBtn.show();
		}
	}
	
	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();
		
		if( tabType == TIMELINE_TAB_RAIN )
		{
			drawImage( tabRain, x, y );
			drawImage( lineRain, x + 145, y + 160 );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			drawImage( tabTemp, x, y );
			drawImage( lineTemp, x + 145, y + 160 );
		}
		
		drawImage( futureBtnBm, x + 1224, y + 107 );
		drawImage( bg, x + 104, y + 73 );
		
		ofDisableAlphaBlending();
	}
	
	void drawImage ( ofImage* image, int x, int y )
	{
		if( image == NULL )
			return;
		
		image->draw( x, y );
	}
	
	//==================================================
	
	void btnPressed ( int & btnId )
	{
		Model :: getInstance()->setTabType( btnId );
	}
	
	void futureBtnPressed ( int & btnId )
	{
		Model :: getInstance()->setFutureBtnPress( btnId );
	}
};