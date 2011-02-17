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
	
	backgroundImage.loadImage( "NawlzFishBowl/fish_bowl_bg.png" );
	particleImage.loadImage( "NawlzFishBowl/fish_bowl_particle.png" );
	bowlImage.loadImage( "NawlzFishBowl/fish_bowl_trace.png" );
	
	ofRectangle	roi;
	roi.x		= 647;
	roi.y		= 80;
	roi.width	= 359;
	roi.height	= 359;
	
	nawlzFishBowl.setROI( roi );
	nawlzFishBowl.createBackgroundTexture( backgroundImage.getPixels(), backgroundImage.width, backgroundImage.height, GL_RGB );
	nawlzFishBowl.createParticleTexture( particleImage.getPixels(), particleImage.width, particleImage.height, GL_RGBA );
	nawlzFishBowl.createBowlTexture( bowlImage.getPixels(), bowlImage.width, bowlImage.height, GL_RGB, roi.x, roi.y );
	nawlzFishBowl.bDrawParticles	= true;
	nawlzFishBowl.bDrawBackground	= true;
	nawlzFishBowl.setup();
	
	bDebug = false;
	
	gui.addToggle( "draw particles",		nawlzFishBowl.bDrawParticles );
	gui.addToggle( "draw background",		nawlzFishBowl.bDrawBackground );
	
	gui.addPage( "fluid" );
	gui.addToggle( "enableRGB",				nawlzFishBowl.fluidEnableRGB );
	gui.addSlider( "fadeSpeed",				nawlzFishBowl.fluidFadeSpeed,			0.0, 0.005 );
	gui.addSlider( "deltaT",				nawlzFishBowl.fluidDeltaT,				0.0, 1.0 );
	gui.addSlider( "visc",					nawlzFishBowl.fluidVisc,				0.0, 0.001 );
	gui.addSlider( "colorDiffusion",		nawlzFishBowl.fluidColorDiffusion,		0.0, 0.1 );
	gui.addSlider( "solverIterations",		nawlzFishBowl.fluidSolverIterations,	5.0, 20 );
	gui.addToggle( "vorticityConfinement",	nawlzFishBowl.fluidEnableVorticityConfinement );
	gui.addToggle( "wrapX",					nawlzFishBowl.fluidWrapX );
	gui.addToggle( "wrapY",					nawlzFishBowl.fluidWrapY );
	
	gui.setAutoSave( false );
	gui.show();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzFishBowlApp :: update()
{
	nawlzFishBowl.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzFishBowlApp :: draw()
{
	ofSetColor( 255, 255, 255, 255 );
	
	nawlzFishBowl.draw();
	
	if( bDebug )
	{
		gui.draw();
	}
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
	nawlzFishBowl.mouseMoved( x, y );
}

void NawlzFishBowlApp :: mouseDragged(int x, int y, int button)
{
	
}

void NawlzFishBowlApp :: mousePressed(int x, int y, int button)
{
	nawlzFishBowl.mousePressed( x, y, button );
}

void NawlzFishBowlApp :: mouseReleased(int x, int y, int button)
{
	nawlzFishBowl.mouseReleased( x, y, button );
}

void NawlzFishBowlApp :: windowResized(int w, int h)
{
	
}