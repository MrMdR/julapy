/*
 *  EventPanel.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 22/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

class EventPanel : public ofxSprite
{

public:
	
	EventPanel()
	{
		setup();
		
		thumbImage = NULL;
	}
	
	~EventPanel() {};
	
	//==================================================

	ofImage*	thumbImage;
	
	ofImage*	bg;
	ofImage*	iconRain;
	ofImage*	iconTemp;
	ofImage*	play;
	ofImage*	shadow;
	
	ofPoint		regPoint;
	ofPoint		bgPoint;
	ofPoint		iconPoint;
	ofPoint		playPoint;
	ofPoint		shadowPoint;
	ofRectangle	whiteBox;
	
	//==================================================
	
	void setup ()
	{
		x = 300;
		y = 300;
		
		bg			= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_bg" );
		iconRain	= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_icon_rain" );
		iconTemp	= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_icon_temp" );
		play		= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_play" );
		shadow		= (ofImage*)ofxAssets :: getInstance()->getAsset( "popup_shadow" );
		
		regPoint.set( 119, 216, 0 );
		bgPoint.set( 0, 0, 0 );
		iconPoint.set( 38, 166, 0 );
		playPoint.set( 53, 36, 0 );
		shadowPoint.set( 40, 214, 0 );
		
		whiteBox.x		= 39;
		whiteBox.y		= 41;
		whiteBox.width	= 155;
		whiteBox.height	= 112;
	}
	
	void setThumbImage ( ofImage* image )
	{
		thumbImage = image;
	}
	
	void update ()
	{
		//
	}
	
	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		ofEnableAlphaBlending();
		
		bg->draw( x + bgPoint.x - regPoint.x, y + bgPoint.y - regPoint.y );
		iconRain->draw( x + iconPoint.x - regPoint.x, y + iconPoint.y - regPoint.y );
		shadow->draw( x + shadowPoint.x - regPoint.x, y + shadowPoint.y - regPoint.y );
		
		ofFill();
		ofSetColor( 0xFFFFFF );
		ofRect( x + whiteBox.x - regPoint.x, y + whiteBox.y - regPoint.y, whiteBox.width, whiteBox.height );
		
		if( thumbImage != NULL )
		{
			thumbImage->draw( x + whiteBox.x - regPoint.x, y + whiteBox.y - regPoint.y );
		}
		
		play->draw( x + playPoint.x - regPoint.x, y + playPoint.y - regPoint.y );
		
		ofFill();
		ofSetColor( 0xFF0000 );
		ofRect( x - 1, y - 1, 3, 3 );
		
		ofDisableAlphaBlending();
	}
	
};