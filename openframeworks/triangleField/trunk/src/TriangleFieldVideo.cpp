/*
 *  TriangleFieldVideo.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 14/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleFieldVideo.h"

TriangleFieldVideo :: TriangleFieldVideo()
{
	videoRect.width  = 640;		// default.
	videoRect.height = 480;
}

TriangleFieldVideo :: ~TriangleFieldVideo()
{

}

void TriangleFieldVideo :: setSize( int w, int h )
{
	screenRect.width  = w;
	screenRect.height = h;
	
	videoRectFs = ofxResizeUtil :: cropToSize( videoRect, screenRect );
}

void TriangleFieldVideo :: loadMovie( string fileName ) 
{
	video.loadMovie( fileName );
	video.setLoopState( true );
	video.play();
	
	videoRect.width	 = video.width;
	videoRect.height = video.height;
	
	videoRectFs = ofxResizeUtil :: cropToSize( videoRect, screenRect );
}

void TriangleFieldVideo :: draw()
{
	ofSetColor( 255, 255, 255, 255 );
	video.draw( videoRectFs.x, videoRectFs.y, videoRectFs.width, videoRectFs.height );
}

float TriangleFieldVideo :: getNormColorValue( int x, int y )
{
	float px, py;		// between 0 - 1.
	float value;
	
	px = ( x - videoRectFs.x ) / (float)videoRectFs.width;
	py = ( y - videoRectFs.y ) / (float)videoRectFs.height;
	
	unsigned char *pixels = video.getPixels();
	
	int w = (int)( ( videoRect.width  - 1 ) * px ) * 3;
	int h = (int)( ( videoRect.height - 1 ) * py );
	int i = h * videoRect.width * 3 + w;
	
	value = pixels[ i + 0 ] / 255.0;
	
	return value;
}