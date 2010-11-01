/*
 *  KeyPanel.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 23/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "Model.h"
#include "KeyDataItem.h"

class KeyPanel : public ofxSprite
{

public:
	
	KeyPanel()
	{
		setup();
	}
	
	~KeyPanel(){}
	
	//==================================================
	
	Model*					model;
	
	ofImage*				image;
	vector<KeyDataItem>		rainColors;
	vector<KeyDataItem>		tempColors;
	ofRectangle				keyRect;
	int						tabType;
	ofTrueTypeFont			font;
	
	//==================================================
	
	void setup ()
	{
		x = 1143;
		y = 184;
		
		model = Model :: getInstance();
		
		image		= (ofImage*)ofxAssets :: getInstance()->getAsset( "key_panel" );
		rainColors	= model->getKeyRainData();
		tempColors	= model->getKeyTempData();
		tabType		= model->getTabType();
		
		keyRect.x		= 83 + x;
		keyRect.y		= 29 + y;
		keyRect.width	= 26;
		keyRect.height	= 184;
		
		font.loadFont( "assets/LIBRARY/fonts/Rockwell.ttf", 9 );
		
		ofAddListener( model->tabTypeChangeEvent, this, &KeyPanel :: tabTypeChanged );
	}
	
	void tabTypeChanged ( int & tabType )
	{
		this->tabType = tabType;
	}
	
	void update ()
	{
		
	}
	
	void draw ()
	{
		ofEnableAlphaBlending();
		
		image->draw( x, y );

		if( tabType == TIMELINE_TAB_RAIN )
		{
			drawKey( rainColors );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			drawKey( tempColors );
		}
		
		ofDisableAlphaBlending();
	}
	
	void drawKey ( vector<KeyDataItem>& data )
	{
		int t = data.size();
		int w = keyRect.width;
		int h = keyRect.height / t;
		
		int fx = x + 68;
		int fy = y + 42;
		
		for( int i=0; i<t; i++ )
		{
			KeyDataItem& dataItem	= data[ i ];
			const ofColor& c		= dataItem.color;
			string keyStr			= dataItem.text;
			
			ofFill();
			ofSetColor( c.r, c.g, c.b, c.a );
			ofRect( keyRect.x, keyRect.y + i * h, keyRect.width, h );
			
			ofRectangle fontRect;
			fontRect = font.getStringBoundingBox( keyStr, 0, 0 );
			
			ofSetColor( 0xFFFFFF );
			font.drawString( keyStr, fx - fontRect.width, fy + i * h );
		}
	}
	
};