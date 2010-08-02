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
}

ClockInfoScreen :: ~ClockInfoScreen ()
{
	//
}

void ClockInfoScreen :: draw ()
{
	if( !bShowing )
		return;
	
	ofEnableAlphaBlending();
	
	ofFill();
	ofSetColor( 0, 0, 0, 100 );
	ofRect( 0, 0, screenWidth, screenHeight );
	
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