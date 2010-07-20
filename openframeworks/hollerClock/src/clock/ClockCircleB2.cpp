/*
 *  ClockCircleB2.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 19/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockCircleB2.h"

ClockCircleB2 :: ClockCircleB2 ( float radius, int color )
{
	set( radius, color );
}

ClockCircleB2 :: ~ClockCircleB2 ()
{

}

void ClockCircleB2 :: set ( float radius, int color )
{
	this->radius	= radius;
	this->colorHex	= color;
	this->color.r	= ( color >> 16 ) & 0xff;
	this->color.g	= ( color >> 8  ) & 0xff;
	this->color.b	= ( color >> 0  ) & 0xff;
}