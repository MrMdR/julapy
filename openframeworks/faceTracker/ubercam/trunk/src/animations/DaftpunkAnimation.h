/*
 *  DaftpunkAnimation.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 22/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFrameAnimation.h"

class DaftpunkAnimation: public ofxFrameAnimation
	{
		
		public :
		
		DaftpunkAnimation();
		~DaftpunkAnimation();
		
		void load();
		void draw( int x, int y, int w, int h );
	};