//
//  NawlzGardenerApp.cpp
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzGardenerApp.h"

#ifndef TARGET_OF_IPHONE

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void NawlzGardenerApp :: setup()
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
    
    img.loadImage( "NawlzQuestions/questions_bg.png" );
    app.createBackgroundTexture( img.getPixels(), img.width, img.height, GL_RGB, 0, 0 );
    img.clear();
    
    img.loadImage( "NawlzQuestions/questions_particle.png" );
    app.createParticle01Texture( img.getPixels(), img.width, img.height, GL_RGBA, 0, 0 );
    img.clear();
    
    app.setup();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzGardenerApp :: update()
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

void NawlzGardenerApp :: draw()
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

void NawlzGardenerApp :: keyPressed(int key)
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

void NawlzGardenerApp :: keyReleased(int key)
{
	app.keyReleased( key );
}

void NawlzGardenerApp :: mouseMoved(int x, int y )
{
	app.mouseMoved( x, y );
}

void NawlzGardenerApp :: mouseDragged(int x, int y, int button)
{
	app.mouseDragged( x, y, button );
}

void NawlzGardenerApp :: mousePressed(int x, int y, int button)
{
	app.mousePressed( x, y, button );
}

void NawlzGardenerApp :: mouseReleased(int x, int y, int button)
{
	app.mouseReleased( x, y, button );
}

void NawlzGardenerApp :: windowResized(int w, int h)
{
	app.windowResized( w, h );
}

#endif