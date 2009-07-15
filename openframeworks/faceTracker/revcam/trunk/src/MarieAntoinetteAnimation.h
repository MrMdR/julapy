/*
 *  MarieAntoinetteAnimation.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 15/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Animation.h"

class MarieAntoinetteAnimation: public Animation
{
		
public :
		
	MarieAntoinetteAnimation();
	~MarieAntoinetteAnimation();
		
	void load();
	void draw( int x, int y, int w, int h );
};