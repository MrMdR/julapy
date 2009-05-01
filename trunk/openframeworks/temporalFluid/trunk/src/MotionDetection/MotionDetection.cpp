/*
 *  MotionDetection.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MotionDetection.h"
#include "testApp.h"

MotionDetection :: MotionDetection()
{
	//
}

MotionDetection :: ~MotionDetection()
{
	//
}

void MotionDetection :: init ( int w, int h )
{
	camWidth	= w;
	camHeight	= h;
	
	cam.initGrabber( camWidth, camHeight );
	
	camVelocityBlur	= 15;
	
	colorImg.allocate( camWidth, camHeight );
	greyNow.allocate( camWidth, camHeight );
	greyBg.allocate( camWidth, camHeight );
	greyBgDiff.allocate( camWidth, camHeight );
	greyPrev.allocate( camWidth, camHeight );
	greyCurDiff.allocate( camWidth, camHeight );
	
	opticalFlow.allocate( camWidth, camHeight );
	
	saveBg		= true;
	newFrame	= false;
	
	reset();
}

void MotionDetection :: update ()
{
	cam.grabFrame();
	
	if( newFrame = cam.isFrameNew() )
	{
		colorImg.setFromPixels( cam.getPixels(), camWidth, camHeight );
//		colorImg.mirror( false, true );
		greyNow.setFromColorImage( colorImg );
		
		if( saveBg )
		{
			greyBg = greyNow;
			saveBg = false;
		}
		
		greyBgDiff.absDiff( greyBg, greyNow );
		
		if( frameCounter > 5 )
		{
			opticalFlow.calc( greyPrev, greyBgDiff, 11 );
			cvSmooth( opticalFlow.vel_x, opticalFlow.vel_x, CV_BLUR , camVelocityBlur );
			cvSmooth( opticalFlow.vel_y, opticalFlow.vel_y, CV_BLUR , camVelocityBlur );
			
			greyCurDiff.absDiff( greyPrev, greyBgDiff );			// curDiff is the difference between the last 2 frames
			greyCurDiff.threshold( 30 );							// chop dark areas
			greyCurDiff.blur();
		}
		else
		{
			frameCounter++;
		}
		
		greyPrev = greyBgDiff;
	}
}

void MotionDetection :: draw ( int x, int y, int w, int h )
{
//	colorImg.draw( x, y, w, h );
//	greyNow.draw( x, y, w, h );
//	greyBg.draw( x, y, w, h );
//	greyBgDiff.draw( x, y, w, h );
//	greyCurDiff.draw( x, y, w, h );

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef( x, y, 0);
	glScalef( 1.0f, 1.0f, 1.0f);
	opticalFlow.draw();
	glPopMatrix();
}

void MotionDetection :: reset()
{
	colorImg.set( 0 );
	greyNow.set( 0 );
	greyBgDiff.set( 0 );
	greyPrev.set( 0 );
	greyCurDiff.set( 0 );
    cvSetZero( opticalFlow.vel_x );
    cvSetZero( opticalFlow.vel_y );
	
	frameCounter = 0;
}

void MotionDetection :: getVelAtPixel( int x, int y, float *u, float *v )
{
	*u = cvGetReal2D( opticalFlow.vel_x, y, x );
	*v = cvGetReal2D( opticalFlow.vel_y, y, x );
}

void MotionDetection :: getVelAtNorm( float x, float y, float *u, float *v )
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

void MotionDetection :: takeBgSnapshot ()
{
	saveBg = true;
}
