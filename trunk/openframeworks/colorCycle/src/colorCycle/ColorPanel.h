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
		
		timeoutStart	= 0;
		timeoutTotal	= 1000;
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
	
	int			timeoutStart;
	int			timeoutTotal;
	bool		bTimeoutPending;
	
	ofxColorPicker* colorPicker0;
	ofxColorPicker* colorPicker1;
	
	//-----------------------------
	
	void setScreen	( ofxScreen screen )
	{
		this->screen = screen;
	};
	
	void show ()
	{
		if( !bShowing )
		{
			tweenTime	= 0;
			bShowing	= true;
			bVisible	= true;
		}
		
		bTimeoutPending = false;
	};
	
	void hide ()
	{
		timeoutStart	= ofGetElapsedTimeMillis();
		bTimeoutPending = true;
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
		if( bTimeoutPending )
		{
			int t1 = timeoutStart + timeoutTotal;
			int t2 = ofGetElapsedTimeMillis();
			
			if( t2 > t1 )
			{
				bTimeoutPending	= false;
				
				tweenTime		= 0;
				bShowing		= false;
			}
		}
		
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
		
		glPushMatrix();
		glTranslatef( x, 0, 0 );
		
		int s  = screen.screenHeight * 0.209;
		int py = 30;
		
		colorPicker0->draw( 0, py, s, s );
		
		py += colorPicker0->getHeight() + 10;
		
		colorPicker1->draw( 0, py, s, s );
		
		glPopMatrix();
	}
};