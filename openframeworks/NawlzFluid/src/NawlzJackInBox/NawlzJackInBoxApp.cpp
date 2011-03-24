/*
 *  NawlzFishBowlApp.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzJackInBoxApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void NawlzJackInBoxApp :: setup()
{
	ofSetFrameRate( 30 );							// OF setup.
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	ofEnableAlphaBlending();
	ofBackground( 0, 0, 0 );
	
	bDebug = false;
	
	gui.addToggle( "bDebug", bDebug );
	
	gui.setAutoSave( false );
	gui.show();
    
    //---
    
    img.loadImage( "NawlzJackInBox/jack_in_box_bg.png" );
    app.createBackgroundTexture	( img.getPixels(), img.width, img.height, GL_RGB, 0, 0 );
    img.clear();
    
    app.setup();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzJackInBoxApp :: update()
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

void NawlzJackInBoxApp :: draw()
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

void NawlzJackInBoxApp :: keyPressed(int key)
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

void NawlzJackInBoxApp :: keyReleased(int key)
{
	app.keyReleased( key );
}

void NawlzJackInBoxApp :: mouseMoved(int x, int y )
{
	app.mouseMoved( x, y );
}

void NawlzJackInBoxApp :: mouseDragged(int x, int y, int button)
{
	app.mouseDragged( x, y, button );
}

void NawlzJackInBoxApp :: mousePressed(int x, int y, int button)
{
	app.mousePressed( x, y, button );
}

void NawlzJackInBoxApp :: mouseReleased(int x, int y, int button)
{
	app.mouseReleased( x, y, button );
}

void NawlzJackInBoxApp :: windowResized(int w, int h)
{
	app.windowResized( w, h );
}