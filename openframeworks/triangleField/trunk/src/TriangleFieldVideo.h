/*
 *  TriangleFieldVideo.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 14/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TRIANGLE_FIELD_VIDEO_H_
#define _TRIANGLE_FIELD_VIDEO_H_

#include "ofMain.h"
#include "ofxResizeUtil.h"

class TriangleFieldVideo
{

public :
	
	TriangleFieldVideo();
	~TriangleFieldVideo();
	
	void setSize( int w, int h );
	void loadMovie( string fileName );
	void draw();
	
private :
	
	ofVideoPlayer	video;
	ofRectangle		videoRect;
	ofRectangle		videoRectFs;
	ofRectangle		screenRect;
};

#endif