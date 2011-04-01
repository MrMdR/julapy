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
    
    //--- bg
    
    img.loadImage( "NawlzJackInBox/jackinbox_bg.png" );
    app.createBackgroundTexture( img.getPixels(), img.width, img.height, GL_RGB, 0, 0 );
    img.clear();
    
    //--- parts.

    img.loadImage( "NawlzJackInBox/jackinbox_body.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 629, 247 ), ofPoint( 0, 0 ), 0, 0, false );
    img.clear();
    
    img.loadImage( "NawlzJackInBox/jackinbox_tongue.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 650, 534 ), ofPoint( 0, 0 ), 0, 0, false );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_lighter.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 759, 330 ), ofPoint( -5, -95 ), 120, 70, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_bomb_holder.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 768, 325 ), ofPoint( -8, -47 ), 70, 80, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_rocket.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 645, 326 ), ofPoint( -130, -47 ), 70, 270, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_arm_right.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 751, 425 ), ofPoint( -8, -95 ), 90, 75, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_arm_left.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 646, 426 ), ofPoint( -115, -73 ), 80, 270, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_bomb.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 723, 310 ), ofPoint( -6, -65 ), 105, 75, true );
    img.clear();

    //--- ears right.
    
    img.loadImage( "NawlzJackInBox/jackinbox_ears_right_3.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 725, 183 ), ofPoint( -4, -13 ), 50, 100, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_ears_right_1.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 724, 180 ), ofPoint( -3, -33 ), 55, 70, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_ears_right_2.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 726, 182 ), ofPoint( -5, -25 ), 70, 90, true );
    img.clear();
    
    //--- brain.

    img.loadImage( "NawlzJackInBox/jackinbox_arrow.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 710, 157 ), ofPoint( -37, -63 ), 35, 0, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_brain.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 714, 138 ), ofPoint( -23, -71 ), 55, 10, true );
    img.clear();

    //--- head.
    
    img.loadImage( "NawlzJackInBox/jackinbox_head_solo.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 667, 134 ), ofPoint( 0, 0 ), 0, 0, false );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_head_spine.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 695, 205 ), ofPoint( 0, 0 ), 0, 0, false );
    img.clear();
    
    //--- eyes left.

    img.loadImage( "NawlzJackInBox/jackinbox_eyes_left_L.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 698, 150 ), ofPoint( -69, -36 ), 60, -70, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_eyes_left_M.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 703, 147 ), ofPoint( -74, -66 ), 60, -45, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_eyes_left_R.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 708, 141 ), ofPoint( -30, -54 ), 45, -20, true );
    img.clear();
    
    //--- eyes right.

    img.loadImage( "NawlzJackInBox/jackinbox_eyes_right_R.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 741, 150 ), ofPoint( -9, -55 ), 80, 60, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_eyes_right_M.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 739, 149 ), ofPoint( -7, -61 ), 55, 36, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_eyes_right_L.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 735, 147 ), ofPoint( -3, -64 ), 55, 13, true );
    img.clear();
    
    //--- ears left.
    
    img.loadImage( "NawlzJackInBox/jackinbox_ears_left_3.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 679, 177 ), ofPoint( -78, -5 ), 75, -125, true );
    img.clear();
    
    img.loadImage( "NawlzJackInBox/jackinbox_ears_left_2.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 678, 178 ), ofPoint( -69, -13 ), 45, -110, true );
    img.clear();

    img.loadImage( "NawlzJackInBox/jackinbox_ears_left_1.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 677, 174 ), ofPoint( -76, -44 ), 70, -75, true );
    img.clear();
    
    //--- mouth.
    
    img.loadImage( "NawlzJackInBox/jackinbox_mouth_base.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 699, 157 ), ofPoint( 0, 0 ), 0, 0, false );
    img.clear();
    
    img.loadImage( "NawlzJackInBox/jackinbox_mouth_tongue.png" );
    app.createBionicPart( img.getPixels(), img.width, img.height, GL_RGBA, ofPoint( 752, 211 ), ofPoint( -12, -7 ), 20, 135, true );
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