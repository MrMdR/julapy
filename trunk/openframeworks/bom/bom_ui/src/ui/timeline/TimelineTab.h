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
	ofImage*			tabRain;
	ofImage*			tabTemp;
	int					tabType;
	
	TimelineTabBtn		rainBtn;
	TimelineTabBtn		tempBtn;
	
	ofEvent<int>		changeEvent;
	
	//==================================================
	
	void setup ()
	{
		bg		= (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_bg" );
		tabRain = (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_rain" );
		tabTemp = (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_temp" );

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
		
		addChild( &rainBtn );
		addChild( &tempBtn );
		
		ofAddListener( rainBtn.btnPressEvent, this, &TimelineTab :: btnPressed );
		ofAddListener( tempBtn.btnPressEvent, this, &TimelineTab :: btnPressed );
		
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
			tabRain->draw( 0, ofGetHeight() - tabRain->height );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			tabTemp->draw( 0, ofGetHeight() - tabTemp->height );
		}
		
		bg->draw( 0, ofGetHeight() - bg->height );
		
		ofDisableAlphaBlending();
	}
	
	//==================================================
	
	void btnPressed ( int & btnId )
	{
		Model :: getInstance()->setTabType( btnId );
	}
};