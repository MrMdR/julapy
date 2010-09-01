/*
 *  ColorPanel.h
 *  iPhoneEmptyExample
 *
 *  Created by Lukasz Karluk on 15/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxScreen.h"
#include "ofxColorPicker.h"
#include "Quad.h"

class ColorPanel
{

public:
	
	ColorPanel()
	{
		colorPicker0	= NULL;
		colorPicker1	= NULL;
		
		bShowing		= false;
		bVisible		= false;
		
		tweenVal		= 0;
		tweenTime		= 0;
		tweenTimeTotal	= 10;
	};
	
	~ColorPanel()
	{
		//
	};
	
	//-----------------------------
	
	bool		bShowing;
	bool		bVisible;
	ofxScreen	screen;
	
	float		tweenVal;
	float		tweenTime;
	float		tweenTimeTotal;
	
	ofxColorPicker* colorPicker0;
	ofxColorPicker* colorPicker1;
	
	ofImage		hint;
	
	//-----------------------------
	
	void setup ()
	{
		hint.loadImage( "images/popup_colors.png" );
	}
	
	void setScreen	( ofxScreen screen )
	{
		this->screen = screen;
	};
	
	void setColorPickers ( ofxColorPicker* cp0, ofxColorPicker* cp1 )
	{
		colorPicker0 = cp0;
		colorPicker1 = cp1;
		
		float x = -0.2	* screen.screenWidth;
		colorPicker0->setPos( x, 0 );
		colorPicker1->setPos( x, 0 );
	}
	
	void show ()
	{
		if( !bShowing )
		{
			tweenTime	= 0;
			bShowing	= true;
			bVisible	= true;
		}
	};
	
	void hide ()
	{
		tweenTime		= 0;
		bShowing		= false;
	};
	
	void toggleShow ()
	{
		if( !bShowing )
		{
			show();
		}
		else
		{
			hide();
		}
	};
	
	void update ()
	{
		if( bShowing && tweenVal != 1.0 )
		{
			tweenVal	= Quad :: easeOut( tweenTime, 0, 1.0, tweenTimeTotal );
			tweenTime	= MIN( tweenTime + 1, tweenTimeTotal );
		}
		else if( !bShowing && tweenVal != 0.0 )
		{
			tweenVal	= 1.0 - Quad :: easeOut( tweenTime, 0.0, 1.0, tweenTimeTotal );
			tweenTime	= MIN( tweenTime + 1, tweenTimeTotal );
		}
		else if( !bShowing && tweenVal == 0.0 )
		{
			bVisible	= false;
		}
	};
	
	void draw ()
	{
		if( !bVisible )
			return;
		
		float x1	= -0.18	* screen.screenWidth;
		float x2	= 0.023 * screen.screenWidth;
		float x		= ( x2 - x1 ) * tweenVal + x1;
		
		int w = screen.screenHeight * 0.228;
		int h = w * 2;
		int y = x2;
		
		ofSetColor( 255, 255, 255, 255 * tweenVal );
		hint.draw( (int)( ( x + w ) * 1.1 ), y );
		
		colorPicker0->draw( x, y, w, h );
		
		y = screen.screenHeight - h - y;
		
		colorPicker1->draw( x, y, w, h );
	}
};