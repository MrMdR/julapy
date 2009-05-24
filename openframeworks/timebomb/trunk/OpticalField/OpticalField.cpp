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

OpticalField :: OpticalField()
{
	//
}

OpticalField :: ~OpticalField()
{
	//
}

void OpticalField :: init ( int w, int h )
{
	camWidth	= w;
	camHeight	= h;
	
	opticalFlowMin		= 2.0f;
	opticalFlowMax		= 10.0f;
	opticalFlowScale	= 1.0f;

	opticalFlowSize		= 5;
	opticalFlowBlur		= 7;
	sourceImgBlur		= 0;
	
	showDifferenceImage	= false;
	
	sourceImg.allocate( camWidth, camHeight );
	resizeImg.allocate( WIDTH, HEIGHT );
	greyNow.allocate( WIDTH, HEIGHT );
	greyPrev.allocate( WIDTH, HEIGHT );
	greyCurDiff.allocate( WIDTH, HEIGHT );
	opticalFlow.allocate( WIDTH, HEIGHT );
	
	reset();
}

void OpticalField :: update ( unsigned char *pixels )
{
	if( camWidth == WIDTH && camHeight == HEIGHT )
	{
		resizeImg.setFromPixels( pixels, WIDTH, HEIGHT );
	}
	else
	{
		sourceImg.setFromPixels( pixels, camWidth, camHeight );
		resizeImg.scaleIntoMe( sourceImg );
	}
	
	resizeImg.mirror( false, true );
	greyNow.setFromColorImage( resizeImg );
	greyNow.blur( sourceImgBlur );

	if( showDifferenceImage )
	{
		greyCurDiff.absDiff( greyPrev, greyNow );
		greyCurDiff.threshold( 30, CV_THRESH_TOZERO );
	}
	
	opticalFlow.calc( greyPrev, greyNow, opticalFlowSize * 2 + 1 );
	cvSmooth( opticalFlow.vel_x, opticalFlow.vel_x, CV_BLUR, opticalFlowBlur * 2 + 1 );
	cvSmooth( opticalFlow.vel_y, opticalFlow.vel_y, CV_BLUR, opticalFlowBlur * 2 + 1 );
		
	greyPrev = greyNow;
}

void OpticalField :: drawCurrentColorImage ( )
{
	resizeImg.draw( 0, 0 );
}

void OpticalField :: drawCurrentGreyImage ( )
{
	greyNow.draw( 0, 0 );
}

void OpticalField :: drawDifference ( )
{
	greyCurDiff.draw( 0, 0 );
}

void OpticalField :: drawOpticalFlow ( )
{
	ofSetColor( 0x000000 );
	ofFill();
	ofRect( 0, 0, WIDTH, HEIGHT );
	
	ofSetColor( 0xffffff );
	ofNoFill();
	
	int x, y;
	float dx, dy;
	int res = 4;
	
	for( y=0; y<HEIGHT; y+=res )
	{
		for( x=0; x<WIDTH; x+=res )
		{
			getVelAtPixel( x, y, &dx, &dy );
			ofLine
			(
				x,
				y,
				x + dx,
				y + dy
			);
		}
	}
}

int OpticalField :: getWidth()
{
	return WIDTH;
}

int OpticalField :: getHeight()
{
	return HEIGHT;
}

void OpticalField :: reset()
{
	sourceImg.set( 0 );
	resizeImg.set( 0 );
	greyNow.set( 0 );
	greyPrev.set( 0 );
	greyCurDiff.set( 0 );
    cvSetZero( opticalFlow.vel_x );
    cvSetZero( opticalFlow.vel_y );
}

void OpticalField :: getVelAtPixel( int x, int y, float *u, float *v )
{
	float dx, dy;
	
	dx = cvGetReal2D( opticalFlow.vel_x, y, x );
	dy = cvGetReal2D( opticalFlow.vel_y, y, x );
	
	// MIN THRESHOLD,
	if( dx < opticalFlowMin && dx > -opticalFlowMin )
		dx = 0;
	
	if( dy < opticalFlowMin && dy > -opticalFlowMin )
		dy = 0;
	
	// MAX CAP.
	if( dx > opticalFlowMax )
		dx = opticalFlowMax;
	
	if( dx < -opticalFlowMax )
		dx = -opticalFlowMax;
	
	if( dy > opticalFlowMax )
		dy = opticalFlowMax;
	
	if( dy < -opticalFlowMax )
		dy = -opticalFlowMax;
	
	// SCALE.
	dx *= opticalFlowScale;
	dy *= opticalFlowScale;
	
	*u = dx;
	*v = dy;
}

void OpticalField :: getVelAtNorm( float x, float y, float *u, float *v )
{
	int ix = x * WIDTH;
	int iy = y * HEIGHT;
	
	if( ix < 0 )
	{
		ix = 0; 
	}
	else if( ix >= WIDTH )
	{
		ix = WIDTH - 1;
	}
	
	if( iy < 0 ) 
	{
		iy = 0; 
	}
	else if( iy >= HEIGHT )
	{
		iy = HEIGHT - 1;
	}
	
	getVelAtPixel( ix, iy, u, v );
}

