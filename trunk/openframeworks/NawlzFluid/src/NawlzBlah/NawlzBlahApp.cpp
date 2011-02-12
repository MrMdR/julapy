/*
 *  NawlzFishBowlApp.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzBlahApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void NawlzBlahApp :: setup()
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

void NawlzBlahApp :: update()
{
	//
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzBlahApp :: draw()
{
	//
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzBlahApp :: keyPressed(int key)
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

void NawlzBlahApp :: keyReleased(int key)
{
	
}

void NawlzBlahApp :: mouseMoved(int x, int y )
{
	//
}

void NawlzBlahApp :: mouseDragged(int x, int y, int button)
{
	
}

void NawlzBlahApp :: mousePressed(int x, int y, int button)
{
	
}

void NawlzBlahApp :: mouseReleased(int x, int y, int button)
{
	
}

void NawlzBlahApp :: windowResized(int w, int h)
{
	
}