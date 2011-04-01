/*
 *  NawlzFishBowlApp.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzBlahApp.h"

#ifndef TARGET_OF_IPHONE

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
	
	backgroundImage.loadImage( "NawlzBlah/blah_bg.png" );
	particleImage.loadImage( "NawlzBlah/blah_particle.png" ); 
	
	blah.createBackgroundTexture( backgroundImage.getPixels(), backgroundImage.getWidth(), backgroundImage.getHeight(), GL_RGB );
	blah.createParticleTexture( particleImage.getPixels(), particleImage.getWidth(), particleImage.getHeight(), GL_RGBA );
	blah.setup();
	
	bDebug = false;
	
	gui.addSlider( "wanderChange",		blah.wanderChange,		0, PI * 0.5 );
	gui.addSlider( "wanderRadius",		blah.wanderRadius,		0, 50 );
	gui.addSlider( "wanderDistance",	blah.wanderDistance,	0, 100 );
	gui.addSlider( "wanderEase",		blah.wanderEase,		0, 1.0 );
	
	gui.setAutoSave( false );
	gui.show();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzBlahApp :: update()
{
	blah.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzBlahApp :: draw()
{
	blah.draw();
	
	if( bDebug )
	{
		gui.draw();
	}
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
	blah.mouseMoved( x, y );
}

void NawlzBlahApp :: mouseDragged(int x, int y, int button)
{
	
}

void NawlzBlahApp :: mousePressed(int x, int y, int button)
{
	blah.mousePressed( x, y, button );
}

void NawlzBlahApp :: mouseReleased(int x, int y, int button)
{
	blah.mouseReleased( x, y, button );
}

void NawlzBlahApp :: windowResized(int w, int h)
{
	
}

#endif