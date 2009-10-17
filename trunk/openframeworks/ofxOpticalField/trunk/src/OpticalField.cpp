/*
 *  MotionDetection.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpticalField.h"

OpticalField :: OpticalField()
{
	setImageType( GL_RGB );
	setMirror( false, true );
}

OpticalField :: ~OpticalField()
{
	//
}

void OpticalField :: init ( const ofRectangle& srcRect, const ofRectangle& dstRect )
{
	this->srcRect = srcRect;
	this->dstRest = dstRect;
	
	opticalFlowMin		= 2.0f;
	opticalFlowMax		= 10.0f;
	opticalFlowScale	= 1.0f;

	opticalFlowSize		= 5;
	opticalFlowBlur		= 7;
	sourceImgBlur		= 0;
	
	showDifferenceImage	= false;
	
	sourceColImg.allocate( srcRect.width, srcRect.height );
	resizeColImg.allocate( dstRest.width, dstRest.height );

	sourceBwImg.allocate( srcRect.width, srcRect.height );
	resizeBwImg.allocate( dstRest.width, dstRest.height );
	
	greyNow.allocate( dstRest.width, dstRest.height );
	greyPrev.allocate( dstRest.width, dstRest.height );
	greyCurDiff.allocate( dstRest.width, dstRest.height );
	opticalFlow.allocate( dstRest.width, dstRest.height );
	
	reset();
}

void OpticalField :: update ( unsigned char *pixels )
{
	if( imageType == GL_RGB )
	{
		if( srcRect.width == dstRest.width && srcRect.height == dstRest.height )
		{
			resizeColImg.setFromPixels( pixels, dstRest.width, dstRest.height );
		}
		else
		{
			sourceColImg.setFromPixels( pixels, srcRect.width, srcRect.height );
			resizeColImg.scaleIntoMe( sourceColImg );
		}
	
		resizeColImg.mirror( false, true );
		greyNow.setFromColorImage( resizeColImg );
	}
	
	if( imageType == GL_LUMINANCE )
	{
		if( srcRect.width == dstRest.width && srcRect.height == dstRest.height )
		{
			greyNow.setFromPixels( pixels, dstRest.width, dstRest.height );
		}
		else
		{
			sourceBwImg.setFromPixels( pixels, srcRect.width, srcRect.height );
			greyNow.scaleIntoMe( sourceBwImg );
		}
		
		greyNow.mirror( mirrorV, mirrorH );
	}
	
	if( sourceImgBlur > 0 )
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

void OpticalField :: setImageType( int value )
{
	imageType = value;
}

void OpticalField :: setMirror( bool mirrorV, bool mirrorH )
{
	this->mirrorV = mirrorV;
	this->mirrorH = mirrorH;
}

void OpticalField :: drawCurrentColorImage ( )
{
	if( imageType == GL_RGB )
	{
		resizeColImg.draw( 0, 0 );
	}
	
	if( imageType == GL_LUMINANCE )
	{
		greyNow.draw( 0, 0 );
	}
}

void OpticalField :: drawCurrentGreyImage ( )
{
	greyNow.draw( 0, 0 );
}

void OpticalField :: drawDifference ( )
{
	greyCurDiff.draw( 0, 0 );
}

void OpticalField :: drawOpticalFlow ( float lineScale )
{
	ofSetColor( 0x000000 );
	ofFill();
	ofRect( 0, 0, dstRest.width, dstRest.height );
	
	ofSetColor( 0xFFFFFF );
	ofNoFill();
	
	float dx, dy;
	int res = 4;
	
	for( int y=0; y<dstRest.height; y+=res )
	{
		for( int x=0; x<dstRest.width; x+=res )
		{
			getVelAtPixel( x, y, &dx, &dy );
			ofLine
			(
				x,
				y,
				x + dx * lineScale,
				y + dy * lineScale
			);
		}
	}
}

int OpticalField :: getWidth()
{
	return dstRest.width;
}

int OpticalField :: getHeight()
{
	return dstRest.height;
}

void OpticalField :: reset()
{
	sourceColImg.set( 0 );
	resizeColImg.set( 0 );
	
	sourceBwImg.set( 0 );
	resizeBwImg.set( 0 );
	
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
	int ix = x * dstRest.width;
	int iy = y * dstRest.height;
	
	if( ix < 0 )
	{
		ix = 0; 
	}
	else if( ix >= dstRest.width )
	{
		ix = dstRest.width - 1;
	}
	
	if( iy < 0 ) 
	{
		iy = 0; 
	}
	else if( iy >= dstRest.height )
	{
		iy = dstRest.height - 1;
	}
	
	getVelAtPixel( ix, iy, u, v );
}


void OpticalField :: getInteractionScale ( float *v )
{
	float dx, dy;
	int res;
	int c;

	res = 2;
	c	= 0;
	
	for( int y=0; y<dstRest.height; y+=res )
	{
		for( int x=0; x<dstRest.width; x+=res )
		{
			getVelAtPixel( x, y, &dx, &dy );
			
			if( dx > 0 || dy > 0 )
			{
				++c;
			}
		}
	}
	
	*v = c / (float)( dstRest.width * dstRest.height / res );
}
