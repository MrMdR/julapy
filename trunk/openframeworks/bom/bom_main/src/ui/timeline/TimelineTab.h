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
#include "SoundManager.h"
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
//	ofImage*			tabRain;
//	ofImage*			tabTemp;
	ofImage*			tabBg;
	ofImage*			futureBtnBmUp;
	ofImage*			futureBtnBmDown;
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
		
		bg				= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bg" );
//		tabRain			= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_rain" );
//		tabTemp			= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_temp" );
		tabBg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_bg" );
		lineRain		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_line_rain" );
		lineTemp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_line_temp" );
		futureBtnBmUp	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_futureBtn_a" );
		futureBtnBmDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_futureBtn_b" );

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
		
		futureBtn.setPos( x + 1100, y + 101 );
		if( futureBtnBmUp != NULL )
			futureBtn.setSize( futureBtnBmUp->width, futureBtnBmUp->height );
		
		addChild( &rainBtn );
		addChild( &tempBtn );
		
		ofAddListener( rainBtn.btnPressEvent,	this, &TimelineTab :: btnPressed );
		ofAddListener( tempBtn.btnPressEvent,	this, &TimelineTab :: btnPressed );
		ofAddListener( futureBtn.btnPressEvent, this, &TimelineTab :: futureBtnPressed );
		
		//-- model.
		
		Model* model;
		model = Model :: getInstance();
		
		tabType = model->getTabType();
	}
	
//	void tadChanged ( int & tabType )
//	{
//		this->tabType = tabType;
//	}
	
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
		
		drawImage( tabBg, x, y );
		
		if( tabType == TIMELINE_TAB_RAIN )
		{
			drawImage( lineRain, x + 145, y + 160 );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			drawImage( lineTemp, x + 145, y + 160 );
		}
		
		if( futureBtn.isMouseDown() || futureBtn.isMouseOver() )
		{
			drawImage( futureBtnBmDown, x + 1100, y + 101 );
		}
		else
		{
			drawImage( futureBtnBmUp, x + 1100, y + 101 );
		}
		
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
	
	void btnPressed ( int & tabType )
	{
		this->tabType = tabType;
		
		Model :: getInstance()->setTabType( tabType );
	}
	
	void futureBtnPressed ( int & btnId )
	{
		Model :: getInstance()->setFutureBtnPress( btnId );
		
		SoundManager* sound;
		sound = SoundManager :: getInstance();
		sound->playClick();
	}
};