/*
 *  BoidForce.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "BoidForce.h"

BoidForce :: BoidForce ( float x, float y, float size, float reach, float magnitude )
{
	this->x			= x;
	this->y			= y;
	this->size		= size;
	this->reach		= reach;
	this->magnitude	= magnitude;
}

BoidForce :: ~BoidForce ()
{

}