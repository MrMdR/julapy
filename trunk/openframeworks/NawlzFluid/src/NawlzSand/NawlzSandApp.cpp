/*
 *  NawlzFishBowlApp.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzSandApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void NawlzSandApp :: setup()
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

void NawlzSandApp :: update()
{
	//
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzSandApp :: draw()
{
	//
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzSandApp :: keyPressed(int key)
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

void NawlzSandApp :: keyReleased(int key)
{
	
}

void NawlzSandApp :: mouseMoved(int x, int y )
{
	//
}

void NawlzSandApp :: mouseDragged(int x, int y, int button)
{
	
}

void NawlzSandApp :: mousePressed(int x, int y, int button)
{
	
}

void NawlzSandApp :: mouseReleased(int x, int y, int button)
{
	
}

void NawlzSandApp :: windowResized(int w, int h)
{
	
}