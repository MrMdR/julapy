/*
 *  KraftwerkAnimation.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 22/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFrameAnimation.h"

class KraftwerkAnimation: public ofxFrameAnimation
	{
		
		public :
		
		KraftwerkAnimation();
		~KraftwerkAnimation();
		
		void load();
		void draw( int x, int y, int w, int h );
	};