/*
 *  NawlzFishBowlApp.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzSandApp.h"

#ifndef TARGET_OF_IPHONE

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
	
	backgroundImage.loadImage( "NawlzSand/sand_bg.png" );
	particleImage.loadImage( "NawlzSand/sand_particle.png" );
	
	nawlzSand.createBackgroundTexture( backgroundImage.getPixels(), backgroundImage.getWidth(), backgroundImage.getHeight(), GL_RGB );
	nawlzSand.createParticleTexture( particleImage.getPixels(), particleImage.getWidth(), particleImage.getHeight(), GL_RGBA );
	nawlzSand.setup();
	
	bDebug = false;
	
	gui.addToggle( "bDebug", bDebug );
	
	gui.setAutoSave( false );
	gui.show();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzSandApp :: update()
{
	nawlzSand.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzSandApp :: draw()
{
	nawlzSand.draw();
	
	if( bDebug )
	{
		gui.draw();
	}
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
	nawlzSand.mouseMoved( x, y );
}

void NawlzSandApp :: mouseDragged(int x, int y, int button)
{
	
}

void NawlzSandApp :: mousePressed(int x, int y, int button)
{
	nawlzSand.mousePressed( x, y, button );
}

void NawlzSandApp :: mouseReleased(int x, int y, int button)
{
	nawlzSand.mouseReleased( x, y, button );
}

void NawlzSandApp :: windowResized(int w, int h)
{
	
}

#endif