/*
 *  Sydfest_01.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 22/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sydfest_01.h"

Sydfest_01 :: Sydfest_01()
{
	//
}

Sydfest_01 :: ~Sydfest_01()
{
	//
}

////////////////////////////////////////////////////////////////
//	SETUP.
////////////////////////////////////////////////////////////////

void Sydfest_01 :: setup ()
{
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	
	bgColor = 73;
	
	initImage();
	initCirclePacker();
	initGui();
}

void Sydfest_01 :: initImage ()
{
//	imageMap.loadImage( "logo.jpg"  );
//	imageMap.loadImage( "logo2.jpg" );
//	imageMap.loadImage( "logo3.jpg" );
	imageMap.loadImage( "meredith.jpg" );
	
	imageMapRect.x		= (int)( ( renderArea.width  - imageMap.width  ) * 0.5 );
	imageMapRect.y		= (int)( ( renderArea.height - imageMap.height ) * 0.5 );
	imageMapRect.width	= imageMap.width;
	imageMapRect.height	= imageMap.height;
	
	imageMapAlpha	= 255;
	imageMapShow	= false;

	//-- image bounds.
	
	imageBounds.loadImage( "meredith.jpg" );
	
	imageBoundsRect.x		= (int)( ( renderArea.width  - imageBounds.width  ) * 0.5 );
	imageBoundsRect.y		= (int)( ( renderArea.height - imageBounds.height ) * 0.5 );
	imageBoundsRect.width	= imageBounds.width;
	imageBoundsRect.height	= imageBounds.height;
	
	imageBoundsAlpha	= 255;
	imageBoundsShow		= false;
}

void Sydfest_01 :: initCirclePacker ()
{
	circleAddedPerFrame	= 0;
	circleRadiusMin		= 50;
	circleRadiusMax		= 100;
	circleGap			= 5.0;
	circleDeathGap		= 5.0;
	circleColorBounds	= false;
	
	circlePacker.setColorMapImage( &imageMap, &imageMapRect );
	circlePacker.setColorBoundsImage( &imageBounds, &imageBoundsRect );
	circlePacker.setCircleRadiusMin( circleRadiusMin );
	circlePacker.setCircleRadiusMax( circleRadiusMax );
	circlePacker.setCircleGap( circleGap );
	circlePacker.setCircleDeathGap( circleDeathGap );
	circlePacker.setCircleColorBounds( circleColorBounds );
	circlePacker.setCircleDeathColor( 0xFFFFFF );
}

void Sydfest_01 :: initGui ()
{
	gui.addSlider( "circleAddedPerFrame",	circleAddedPerFrame,	0, 50  );
	gui.addSlider( "circleRadiusMin",		circleRadiusMin,		0, 50  );
	gui.addSlider( "circleRadiusMax",		circleRadiusMax,		2, 100 );
	gui.addSlider( "circleGap",				circleGap,				0, 5   );
	gui.addSlider( "circleDeathGap",		circleDeathGap,			0, 5   );
	gui.addToggle( "circleColorBounds",		circleColorBounds			   );
	
	gui.addToggle( "imageMapShow",			imageMapShow				   );
	gui.addSlider( "imageMapAlpha",			imageMapAlpha,			0, 255 );

	gui.addToggle( "imageBoundsShow",		imageBoundsShow				   );
	gui.addSlider( "imageBoundsAlpha",		imageBoundsAlpha,		0, 255 );
	
	gui.addSlider( "bgColor",				bgColor,				0, 255 );
	
//	gui.loadFromXML( "ofxSimpleGuiToo.xml" );
}

////////////////////////////////////////////////////////////////
//	UPDATE.
////////////////////////////////////////////////////////////////

void Sydfest_01 :: update ()
{
	circlePacker.setCircleRadiusMin( circleRadiusMin );
	circlePacker.setCircleRadiusMax( circleRadiusMax );
	circlePacker.setCircleGap( circleGap );
	circlePacker.setCircleDeathGap( circleDeathGap );
	circlePacker.setCircleColorBounds( circleColorBounds );
	
	for( int i=0; i<circleAddedPerFrame; i++ )
	{
		circlePacker.addCircle();
	}
	
	circlePacker.update();
}

////////////////////////////////////////////////////////////////
//	DRAW.
////////////////////////////////////////////////////////////////

void Sydfest_01 :: draw ()
{
	ofBackground( bgColor, bgColor, bgColor );
	
	if( imageMapShow )
	{
		ofEnableAlphaBlending();
		ofSetColor( 255, 255, 255, imageMapAlpha );
		imageMap.draw( imageMapRect.x, imageMapRect.y );
		ofDisableAlphaBlending();
	}

	if( imageBoundsShow )
	{
		ofEnableAlphaBlending();
		ofSetColor( 255, 255, 255, imageBoundsAlpha );
		imageBounds.draw( imageBoundsRect.x, imageBoundsRect.y );
		ofDisableAlphaBlending();
	}
	
	circlePacker.draw();
}

void Sydfest_01 :: drawDebug ()
{
	gui.draw();
}

////////////////////////////////////////////////////////////////
//	HANDLERS.
////////////////////////////////////////////////////////////////

void Sydfest_01 :: keyReleased ( int key )
{
	if( key == 'r' )
	{
		circlePacker.reset();
		circlePacker.setColorMapImage( &imageMap, &imageMapRect );
		circlePacker.setColorBoundsImage( &imageBounds, &imageBoundsRect );
	}
	
	if( key == 'w' )
		circlePacker.writeToFile();
	
	if( key == 'l' )
		circlePacker.loadFromFile();
	
	if( key == 'p' )
		circlePacker.togglePause();
}