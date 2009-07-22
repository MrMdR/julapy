/*
 *  MarieAntoinetteAnimation.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFrameAnimation.h"

class MarieAntoinetteAnimation: public ofxFrameAnimation
{
		
public :
		
	MarieAntoinetteAnimation();
	~MarieAntoinetteAnimation();
		
	void load();
	void draw( int x, int y, int w, int h );
};