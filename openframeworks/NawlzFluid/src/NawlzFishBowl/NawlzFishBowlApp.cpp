/*
 *  NawlzFishBowlApp.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzFishBowlApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void NawlzFishBowlApp :: setup()
{
	ofSetFrameRate( 30 );							// OF setup.
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	ofEnableAlphaBlending();
	ofBackground( 0, 0, 0 );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzFishBowlApp :: update()
{
	//
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzFishBowlApp :: draw()
{
	//
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzFishBowlApp :: keyPressed(int key)
{
	if( key >= '0' && key <= '9' )
	{
		gui.setPage( key - '0' );
		gui.show();
	}
	
	switch(key)
	{
		case '[': gui.prevPage(); break;
		case ']': gui.nextPage(); break;
	}
	
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
}

void NawlzFishBowlApp :: keyReleased(int key)
{
	
}

void NawlzFishBowlApp :: mouseMoved(int x, int y )
{
	//
}

void NawlzFishBowlApp :: mouseDragged(int x, int y, int button)
{
	
}

void NawlzFishBowlApp :: mousePressed(int x, int y, int button)
{
	
}

void NawlzFishBowlApp :: mouseReleased(int x, int y, int button)
{
	
}

void NawlzFishBowlApp :: windowResized(int w, int h)
{
	
}