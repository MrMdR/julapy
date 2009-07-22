/*
 *  HeadphoneAnimation.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 22/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFrameAnimation.h"

class HeadphoneAnimation: public ofxFrameAnimation
{
		
public :
		
	HeadphoneAnimation();
	~HeadphoneAnimation();
		
	void load();
	void draw( int x, int y, int w, int h );
};