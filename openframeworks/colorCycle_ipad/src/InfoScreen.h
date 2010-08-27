/*
 *  InfoScreen.h
 *  iPhoneEmptyExample
 *
 *  Created by Lukasz Karluk on 26/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class InfoScreen
{
	
public:
	
	InfoScreen()
	{
		bShow = false;
	}
	
	//---------------------------
	
	ofImage		image;
	bool		bShow;
	
	//---------------------------
	
	void setup ()
	{
		image.loadImage( "info_screen_0.png" );
	}
	
	void show ()
	{
		bShow = true;
	}
	
	void hide ()
	{
		bShow = false;
	}
	
	void toggle ()
	{
		bShow = !bShow;
	}
	
	void draw ()
	{
		if( !bShow )
			return;
		
		int x = (int)( ( ofGetWidth()  - image.width  ) * 0.5 );
		int y = (int)( ( ofGetHeight() - image.height ) * 0.5 );
		
		ofSetColor( 255, 255, 255, 255 );
		image.draw( x, y );
	}
	
	void launchJulapySite ()
	{
		launchSite( "http://julapy.com/blog" );
	}
	
	void launchEliSite()
	{
		launchSite( "http://gentleforce.angrypixel.org/" );
	}
	
	void launchSite ( string url )
	{
		[ [ UIApplication sharedApplication ] openURL:[NSURL URLWithString: [[[[NSString alloc] initWithCString: url.c_str()]stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding] autorelease] ]];
	}
};