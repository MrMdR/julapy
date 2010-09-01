/*
 *  SplashScreen.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 29/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class SplashScreen
{

public:
	
	SplashScreen()
	{
		startFrame	= 30;
		totalFrames	= 30;
		bComplete	= false;
	};
	
	//-------------------------------
	
	ofImage		image;
	int			startFrame;
	int			totalFrames;
	bool		bComplete;
	
	//-------------------------------
	
	void setup ()
	{
		image.loadImage( "Default.png" );
	};

	void draw ()
	{
		if( bComplete )
			return;
		
		int f, t;
		f = ofGetFrameNum();
		t = totalFrames;
		
		float p, a;
		
		if( f < startFrame )
		{
			a = 1.0;
		}
		else
		{
			p = ( f - startFrame ) / (float)t;
			a = 1 - p;
		}
		
		int cx = (int)( ofGetScreenWidth()  * 0.5 );
		int cy = (int)( ofGetScreenHeight() * 0.5 );
		
		ofEnableAlphaBlending();
		ofSetColor( 255, 255, 255, 255 * a );
		
		glPushMatrix();
		glTranslatef( cx, cy, 0 );
		glRotatef( 90, 0, 0, 1 );
		glTranslatef( -cy, -cx, 0 );
		image.draw( 0, 0 );
		glPopMatrix();
		
		ofDisableAlphaBlending();
		
		if( p == 1.0 )
		{
			bComplete = true;
			image.clear();
		}
	};
};