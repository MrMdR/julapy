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