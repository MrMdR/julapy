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
	
	~InfoScreen ()
	{
		image.clear();
	}
	
	//---------------------------
	
	ofImage			image;
	ofRectangle		imageRect;
	ofRectangle		julapyRect;
	ofRectangle		eliRect;
	
	//---------------------------
	
	void setup ()
	{
		image.loadImage( "images/popup_info.png" );
		
		imageRect.x			= (int)( ( ofGetWidth()  - image.width  ) * 0.5 );
		imageRect.y			= (int)( ( ofGetHeight() - image.height ) * 0.5 );
		imageRect.width		= image.width;
		imageRect.height	= image.height;
		
		julapyRect.x		= (int)( imageRect.x + 10 );
		julapyRect.y		= (int)( imageRect.y + 100 );
		julapyRect.width	= (int)( imageRect.width * 0.5 );
		julapyRect.height	= (int)( imageRect.height * 0.05 );
		
		eliRect.x			= (int)( imageRect.x + 10 );
		eliRect.y			= (int)( imageRect.y + 250 );
		eliRect.width		= (int)( imageRect.width * 0.5 );
		eliRect.height		= (int)( imageRect.height * 0.05 );
	}
	
	void show ()
	{
		if( bShow )
			return;
		
		AnimScreen :: show();
	}
	
	void hide ()
	{
		AnimScreen :: hide();
	}
	
	void mouseDown ( int x, int y )
	{
		if( bShow )
		{
			if( !hitTest( x, y, imageRect ) )
			{
				hide();
			}
			
			if( hitTest( x, y, julapyRect ) )
			{
				launchSite( "http://julapy.com/blog" );
			}
			
			if( hitTest( x, y, eliRect ) )
			{
//				launchSite( "http://gentleforce.angrypixel.org/" );
//				launchSite( "http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewAlbum?id=367193275" );		// album link.
				launchSite( "http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewArtist?id=367193276" );		// artist link.
			}
		}
	}
	
	bool hitTest ( int x, int y, const ofRectangle& rect )
	{
		bool b1 = x >= rect.x;
		bool b2 = x <  rect.x + rect.width;
		bool b3 = y >= rect.y;
		bool b4 = y <  rect.y + rect.height;
		
		return ( b1 && b2 && b3 && b4 );
	}
	
	void draw ()
	{
		if( !bVisible )
			return;
		
		ofEnableAlphaBlending();
		
		ofSetColor( 255, 255, 255, alpha );
		image.draw( imageRect.x, imageRect.y );
		
		ofFill();
		ofSetColor( 255, 0, 0, 50 );
		ofRect( julapyRect.x, julapyRect.y, julapyRect.width, julapyRect.height );
		ofRect( eliRect.x, eliRect.y, eliRect.width, eliRect.height );
		
		ofDisableAlphaBlending();
	}
	
	void launchSite ( string url )
	{
		[ [ UIApplication sharedApplication ] openURL:[NSURL URLWithString: [[[[NSString alloc] initWithCString: url.c_str()]stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding] autorelease] ]];
	}
};