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
	ofBackground( 60, 60, 60 );
	
	initImage();
	initCirclePacker();
	initGui();
}

void Sydfest_01 :: initImage ()
{
//	image.loadImage( "logo.jpg"  );
//	image.loadImage( "logo2.jpg" );
	image.loadImage( "logo3.jpg" );
	
	imageRect.x			= (int)( ( renderArea.width  - image.width  ) * 0.5 );
	imageRect.y			= (int)( ( renderArea.height - image.height ) * 0.5 );
	imageRect.width		= image.width;
	imageRect.height	= image.height;
	
	bImage = false;
}

void Sydfest_01 :: initCirclePacker ()
{
	circleAddedPerFrame	= 1;
	circleRadiusMin		= 2;
	circleRadiusMax		= 100;
	circleGap			= 2.0;
	circleColorBounds	= true;
	
	circlePacker.setImage( &image, &imageRect );
	circlePacker.setCircleRadiusMin( circleRadiusMin );
	circlePacker.setCircleRadiusMax( circleRadiusMax );
	circlePacker.setCircleGap( circleGap );
	circlePacker.setCircleColorBounds( circleColorBounds );
}

void Sydfest_01 :: initGui ()
{
	gui.addSlider( "circleAddedPerFrame",	circleAddedPerFrame,	0, 50  );
	gui.addSlider( "circleRadiusMin",		circleRadiusMin,		0, 10  );
	gui.addSlider( "circleRadiusMax",		circleRadiusMax,		2, 100 );
	gui.addSlider( "circleGap",				circleGap,				0, 5   );
	gui.addToggle( "circleColorBounds",		circleColorBounds			   );
	
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
	if( bImage )
	{
		ofSetColor( 0xFFFFFF );
		image.draw( imageRect.x, imageRect.y );
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
	if( key == 'i' )
		bImage = !bImage;
	
	if( key == 'r' )
	{
		circlePacker.reset();
		circlePacker.setImage( &image, &imageRect );
	}
	
	if( key == 'w' )
		circlePacker.writeToFile();
	
	if( key == 'l' )
		circlePacker.loadFromFile();
	
	if( key == 'p' )
		circlePacker.togglePause();
}