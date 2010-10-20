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
#include "TimelineTabBtn.h"

#define TIMELINE_TAB_RAIN	0
#define TIMELINE_TAB_TEMP	1

class TimelineTab : public ofxSprite
{

public:
	
	TimelineTab()
	{
		setup();
	};
	
	~TimelineTab() {};
	
	//==================================================
	
	ofImage*			tabRain;
	ofImage*			tabTemp;
	int					tabType;
	
	TimelineTabBtn		rainBtn;
	TimelineTabBtn		tempBtn;
	
	ofEvent<int>		changeEvent;
	
	//==================================================
	
	void setup ()
	{
		tabRain = (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_rain" );
		tabTemp = (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_temp" );
		
		rainBtn.setIcon( (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_rain_icon" ) );
		tempBtn.setIcon( (ofImage*)ofxAssets :: getInstance()->getAsset( "timeline_tab_temp_icon" ) );

		rainBtn.x = 0;
		rainBtn.y = 578;
		
		tempBtn.x = 233;
		tempBtn.y = 578;
		
		rainBtn.setup();
		tempBtn.setup();
		
		rainBtn.setID( TIMELINE_TAB_RAIN );
		tempBtn.setID( TIMELINE_TAB_TEMP );
		
		rainBtn.show();
		tempBtn.hide();
		
		addChild( &rainBtn );
		addChild( &tempBtn );
		
		ofAddListener( rainBtn.btnPressEvent, this, &TimelineTab :: btnPressed );
		ofAddListener( tempBtn.btnPressEvent, this, &TimelineTab :: btnPressed );
		
		tabType = TIMELINE_TAB_RAIN;
	}
	
	void setTabType ( int i )
	{
		bool bChange;
		bChange = tabType != i;
		
		tabType = i;
		
		if( bChange )
		{
			ofNotifyEvent( changeEvent, this->tabType, this );
		}
	}
	
	int getTabType ()
	{
		return tabType;
	}
	
	void toggleTabs ()
	{
		if( tabType == TIMELINE_TAB_RAIN )
		{
			tabType = TIMELINE_TAB_TEMP;
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			tabType = TIMELINE_TAB_RAIN;
		}
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
		ofEnableAlphaBlending();
		
		if( tabType == TIMELINE_TAB_RAIN )
		{
			tabRain->draw( 0, ofGetHeight() - tabRain->height );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			tabTemp->draw( 0, ofGetHeight() - tabTemp->height );
		}
		
		ofDisableAlphaBlending();
	}
	
	//==================================================
	
	void btnPressed ( int & btnId )
	{
		setTabType( btnId );
	}
};