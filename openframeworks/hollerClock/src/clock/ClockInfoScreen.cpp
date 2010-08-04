/*
 *  ClockInfoScreen.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 31/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockInfoScreen.h"

ClockInfoScreen :: ClockInfoScreen ()
{
	screenWidth		= ofGetWidth();
	screenHeight	= ofGetHeight();
	
	bShowing		= false;
	
	tex				= NULL;
}

ClockInfoScreen :: ~ClockInfoScreen ()
{
	//
}

void ClockInfoScreen :: setSize ( int w, int h )
{
	screenWidth		= w;
	screenHeight	= h;
}

void ClockInfoScreen :: setTexture ( ofTexture* tex )
{
	this->tex = tex;
}

void ClockInfoScreen :: draw ()
{
	if( !bShowing )
		return;
	
	ofEnableAlphaBlending();
	
	ofFill();
	ofSetColor( 0, 0, 0, 100 );
	ofRect( 0, 0, screenWidth, screenHeight );
	
	if( tex != NULL )
	{
		ofSetColor( 255, 255, 255, 255 );
		tex->draw( 0, 0 );
	}
	
	ofDisableAlphaBlending();
}

void ClockInfoScreen :: show ()
{
	if( !bShowing )
	{
		bShowing = true;
	}
}

void ClockInfoScreen :: hide ()
{
	if( bShowing )
	{
		bShowing = false;
	}
}