/*
 *  ClockCircle.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLOCK_CIRCLE_H
#define CLOCK_CIRCLE_H

#include "ClockCircleB2.h"

class ClockCircle : public ClockCircleB2
{

public :
	
	 ClockCircle	( float radius = 0.001, int color = 0xFFFFFF );
	~ClockCircle	();

	void enableGravity	( bool b );
	void draw			();
};

#endif