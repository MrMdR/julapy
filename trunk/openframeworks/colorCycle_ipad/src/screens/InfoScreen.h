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
#include "AnimScreen.h"

class InfoScreen : public AnimScreen
{
	
public:
	
	InfoScreen()
	{
		//
	}
	
	//---------------------------
	
	ofImage image;
	
	//---------------------------
	
	void setup ()
	{
		image.loadImage( "images/popup_info.png" );
	}
	
	void draw ()
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