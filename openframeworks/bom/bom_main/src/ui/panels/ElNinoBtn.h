/*
 *  ElNinoBtn.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 27/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "Model.h" 
#include "SoundManager.h"
#include "Btn.h"

class ElNinoBtn : public ofxSprite
{

public:
	
	ElNinoBtn()
	{
		setup();
	}
	
	~ElNinoBtn()
	{
		
	}
	
	//==================================================

	Model*		model;
	ofImage*	iconUp;
	ofImage*	iconDown;
	Btn			btn;
	
	//==================================================
	
	void setup ()
	{
		model		= Model :: getInstance();
		iconUp		= (ofImage*)ofxAssets :: getInstance()->getAsset( "map_elnino_icon_up" );
		iconDown	= (ofImage*)ofxAssets :: getInstance()->getAsset( "map_elnino_icon_over" );
		
		x = 990;
		y = 210;
		
		btn.setPos( x, y );
		if( iconUp != NULL )
		{
			btn.setSize( iconUp->width, iconUp->height );
		}
		
		ofAddListener( btn.btnPressEvent, this, &ElNinoBtn :: btnPressed );
	}
	
	void btnPressed ( int& btnID )
	{
		model->setElninoBtnPress();
		
		SoundManager* sound;
		sound = SoundManager :: getInstance();
		sound->playClick();
	}
	
	void update ()
	{
		//
	}
	
	void draw ()
	{
		ofEnableAlphaBlending();
		
		bool bMouseDown;
		bMouseDown = ( btn.isMouseDown() || btn.isMouseOver() );
		
		if( bMouseDown )
		{
			if( iconDown != NULL )
			{
				iconDown->draw( x, y );
			}
		}
		else
		{
			if( iconUp != NULL )
			{
				iconUp->draw( x, y );
			}
		}
		
		ofDisableAlphaBlending();
	}
};