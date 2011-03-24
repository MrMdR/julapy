/*
 *  NawlzFishBowlApp.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzInfodelApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void NawlzInfodelApp :: setup()
{
	ofSetFrameRate( 30 );							// OF setup.
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	ofEnableAlphaBlending();
	ofBackground( 0, 0, 0 );
	
	bDebug = false;
	
	gui.addToggle( "bDebug", bDebug );
    gui.addSlider( "connectionLength",  app.connectionLength,   0, 100 );
    gui.addSlider( "connectionAlpha",   app.connectionAlpha,    0, 1.0 );
	
	gui.setAutoSave( false );
	gui.show();
    
    //---
    
    img.loadImage( "NawlzInfodel/infodel_bg_clean.png" );
    app.createBackgroundTexture( img.getPixels(), img.width, img.height, GL_RGB, 0, 0 );
    img.clear();

    img.loadImage( "NawlzInfodel/infodel_harley_0.png" );
    app.createHarleyOneTexture( img.getPixels(), img.width, img.height, GL_RGBA, 366, 311 );
    img.clear();

    img.loadImage( "NawlzInfodel/infodel_harley_1.png" );
    app.createHarleyTwoTexture( img.getPixels(), img.width, img.height, GL_RGBA, 438, 366 );
    img.clear();
    
    img.loadImage( "NawlzInfodel/infodel_particle.png" );
    app.createParticleTexture( img.getPixels(), img.width, img.height, GL_RGBA, 0, 0 );
    img.clear();
    
    app.setup();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzInfodelApp :: update()
{
    if( bDebug )
        gui.show();
    else
        gui.hide();
    
    app.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzInfodelApp :: draw()
{
	app.draw();
	
	if( bDebug )
	{
		gui.draw();
	}
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzInfodelApp :: keyPressed(int key)
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
    
    app.keyPressed( key );
}

void NawlzInfodelApp :: keyReleased(int key)
{
	app.keyReleased( key );
}

void NawlzInfodelApp :: mouseMoved(int x, int y )
{
	app.mouseMoved( x, y );
}

void NawlzInfodelApp :: mouseDragged(int x, int y, int button)
{
	app.mouseDragged( x, y, button );
}

void NawlzInfodelApp :: mousePressed(int x, int y, int button)
{
	app.mousePressed( x, y, button );
    app.toggleHarley();
}

void NawlzInfodelApp :: mouseReleased(int x, int y, int button)
{
	app.mouseReleased( x, y, button );
}

void NawlzInfodelApp :: windowResized(int w, int h)
{
	app.windowResized( w, h );
}

