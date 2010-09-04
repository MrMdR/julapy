/*
 *  PopupScreen.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 1/09/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "AnimScreen.h"

class PopupScreen : public AnimScreen
{
public:
	
	PopupScreen()
	{
		time		= 0;
		timeout		= 0;
		bTimeout	= false;
	}
	
	~PopupScreen()
	{
		image.clear();
	}
	
	//---------------------------
	
	ofImage image;
	int		time;
	int		timeout;
	bool	bTimeout;
	
	//---------------------------
	
	virtual void setup ()
	{
		image.loadImage( "images/popup_saved.png" );
	}
	
	virtual void show ()
	{
		AnimScreen :: show();
		
		time		= ofGetElapsedTimeMillis();
		timeout		= time + 1500;
		bTimeout	= true;
	}
	
	virtual void update ()
	{
		AnimScreen :: update();
		
		if( bTimeout )
		{
			if( ofGetElapsedTimeMillis() > timeout )
			{
				bTimeout = false;
				
				hide();
			}
		}
	}
	
	virtual void draw ()
	{
		if( !bVisible )
			return;
		
		int x = (int)( ( ofGetWidth()  - image.width  ) * 0.5 );
		int y = (int)( ( ofGetHeight() - image.height ) * 0.5 );
		
		ofEnableAlphaBlending();
		ofSetColor( 255, 255, 255, alpha );
		image.draw( x, y );
		ofDisableAlphaBlending();
	}
};