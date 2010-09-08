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
	ofRectangle		itunesRect;
	ofRectangle		openfRect;
	
	//---------------------------
	
	void setup ()
	{
		image.loadImage( "images/popup_info.png" );
		
		imageRect.x			= (int)( ( ofGetWidth()  - image.width  ) * 0.5 );
		imageRect.y			= (int)( ( ofGetHeight() - image.height ) * 0.5 );
		imageRect.width		= image.width;
		imageRect.height	= image.height;
		
		julapyRect.x		= (int)( imageRect.x + 151 );
		julapyRect.y		= (int)( imageRect.y + 45 );
		julapyRect.width	= (int)( 159 );
		julapyRect.height	= (int)( 20 );
		
		eliRect.x			= (int)( imageRect.x + 181 );
		eliRect.y			= (int)( imageRect.y + 25 );
		eliRect.width		= (int)( 151 );
		eliRect.height		= (int)( 20 );
		
		itunesRect.x		= (int)( imageRect.x + 119 );
		itunesRect.y		= (int)( imageRect.y + 226 );
		itunesRect.width	= (int)( 69 );
		itunesRect.height	= (int)( 69 );

		openfRect.x			= (int)( imageRect.x + 266 );
		openfRect.y			= (int)( imageRect.y + 281 );
		openfRect.width		= (int)( 107 );
		openfRect.height	= (int)( 21 );
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
//			if( !hitTest( x, y, imageRect ) )
//			{
//				hide();
//			}
			
			if( hitTest( x, y, julapyRect ) )
			{
				launchSite( "http://julapy.com/blog" );
			}
			
			if( hitTest( x, y, eliRect ) )
			{
				launchSite( "http://gentleforce.angrypixel.org/" );
			}
			
			if( hitTest( x, y, itunesRect ) )
			{
//				launchSite( "http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewAlbum?id=367193275" );		// album link.
				launchSite( "http://phobos.apple.com/WebObjects/MZStore.woa/wa/viewArtist?id=367193276" );		// artist link.
			}
			
			if( hitTest( x, y, openfRect ) )
			{
				launchSite( "http://www.openframeworks.cc/" );
			}
			
			hide();
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
//		ofRect( julapyRect.x, julapyRect.y, julapyRect.width, julapyRect.height );
//		ofRect( eliRect.x, eliRect.y, eliRect.width, eliRect.height );
		
		ofDisableAlphaBlending();
	}
	
	void launchSite ( string url )
	{
		[ [ UIApplication sharedApplication ] openURL:[NSURL URLWithString: [[[[NSString alloc] initWithCString: url.c_str()]stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding] autorelease] ]];
	}
};