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
	bVisible		= false;
	
	tex				= NULL;
	
	alphaBg			= 0;
	alphaTex		= 0;
	tweenTime		= 0;
	tweenTimeTotal	= 20;
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

void ClockInfoScreen :: update ()
{
	if( bShowing && alphaBg != 1.0 )
	{
		alphaBg		= Quad :: easeOut( tweenTime, 0, 1.0, tweenTimeTotal );
		tweenTime	= MIN( tweenTime + 1, tweenTimeTotal );
	}
	else if( !bShowing && alphaBg != 0.0 )
	{
		alphaBg		= 1 - Quad :: easeOut( tweenTime, 0.0, 1.0, tweenTimeTotal );
		tweenTime	= MIN( tweenTime + 1, tweenTimeTotal );
	}
	else if( !bShowing && alphaBg == 0.0 )
	{
		bVisible	= false;
	}

}

void ClockInfoScreen :: draw ()
{
	if( !bVisible )
		return;
	
	ofEnableAlphaBlending();
	
	ofFill();
	ofSetColor( 0, 0, 0, alphaBg * 100 );
	ofRect( 0, 0, screenWidth, screenHeight );
	
	if( tex != NULL )
	{
		ofSetColor( 255, 255, 255, alphaBg * 255 );
		tex->draw( 0, 0 );
	}
	
	ofDisableAlphaBlending();
}

void ClockInfoScreen :: show ()
{
	if( !bShowing )
	{
		tweenTime	= 0;
		bShowing	= true;
		bVisible	= true;
	}
}

void ClockInfoScreen :: hide ()
{
	if( bShowing )
	{
		tweenTime	= 0;
		bShowing	= false;
	}
}