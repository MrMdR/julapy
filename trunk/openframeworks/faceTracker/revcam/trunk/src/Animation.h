/*
 *  Animation.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 14/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ANIMATION_H
#define	_ANIMATION_H

#include "ofMain.h"

class Animation
{

public :
	Animation();
	~Animation();
	
	void addFrame( string imagePath );
	virtual void draw( int x, int y, int w, int h );
	void reset();
	void nextFrame();
	void prevFrame();
	void gotoFrame( int frame );
	int  totalFrames();
	int  width();
	int	 height();
	
	vector<ofImage>	images;
	int	imageIndex;
	
};

#endif