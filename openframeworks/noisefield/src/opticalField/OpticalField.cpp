/*
 *  MotionDetection.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpticalField.h"
#include "testApp.h"

OpticalField :: OpticalField( int w, int h )
{
	camWidth	= w;
	camHeight	= h;

	cam.initGrabber( camWidth, camHeight );
	
	colorImg.allocate( camWidth, camHeight );
	greyNow.allocate( camWidth, camHeight );
	greyPrev.allocate( camWidth, camHeight );
	greyCurDiff.allocate( camWidth, camHeight );
	opticalFlow.allocate( camWidth, camHeight );
	
	newFrame	= false;
	
	reset();
}

OpticalField :: ~OpticalField()
{
	//
}

void OpticalField :: update ()
{
	cam.grabFrame();
	
	if( newFrame = cam.isFrameNew() )
	{
		colorImg.setFromPixels( cam.getPixels(), camWidth, camHeight );
		colorImg.mirror( false, true );
		greyNow.setFromColorImage( colorImg );
		
		opticalFlow.calc( greyPrev, greyNow, 11 );
		cvSmooth( opticalFlow.vel_x, opticalFlow.vel_x, CV_BLUR , 15 );
		cvSmooth( opticalFlow.vel_y, opticalFlow.vel_y, CV_BLUR , 15 );
			
		greyCurDiff.absDiff( greyPrev, greyNow );
		greyCurDiff.threshold( 30, CV_THRESH_TOZERO );
		greyCurDiff.blur( 3 );
		
		greyPrev = greyNow;
	}
}

void OpticalField :: drawCurrentColorImage ( int x, int y, int w, int h )
{
	colorImg.draw( x, y, w, h );
}

void OpticalField :: drawCurrentGreyImage ( int x, int y, int w, int h )
{
	greyNow.draw( x, y, w, h );
}

void OpticalField :: drawDifference ( int x, int y, int w, int h )
{
	greyCurDiff.draw( x, y, w, h );
}

void OpticalField :: drawOpticalFlow ( int x, int y, int w, int h )
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef( x, y, 0);
	opticalFlow.draw();
	glPopMatrix();
}

void OpticalField :: reset()
{
	colorImg.set( 0 );
	greyNow.set( 0 );
	greyPrev.set( 0 );
	greyCurDiff.set( 0 );
    cvSetZero( opticalFlow.vel_x );
    cvSetZero( opticalFlow.vel_y );
}

void OpticalField :: getVelAtPixel( int x, int y, float *u, float *v )
{
	*u = cvGetReal2D( opticalFlow.vel_x, y, x );
	*v = cvGetReal2D( opticalFlow.vel_y, y, x );
}

void OpticalField :: getVelAtNorm( float x, float y, float *u, float *v )
{
	int ix = x * camWidth;
	int iy = y * camHeight;
	
	if( ix < 0 )
	{
		ix = 0; 
	}
	else if( ix >= camWidth )
	{
		ix = camWidth - 1;
	}
	
	if( iy < 0 ) 
	{
		iy = 0; 
	}
	else if( iy >= camHeight )
	{
		iy = camHeight - 1;
	}
	
	*u = cvGetReal2D( opticalFlow.vel_x, iy, ix );
	*v = cvGetReal2D( opticalFlow.vel_y, iy, ix );
}

