/*
 *  SarkAnimation.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFrameAnimation.h"

class SarkoAnimation: public ofxFrameAnimation
{
		
public :
		
	SarkoAnimation();
	~SarkoAnimation();
		
	void load();
	void draw( int x, int y, int w, int h );
};