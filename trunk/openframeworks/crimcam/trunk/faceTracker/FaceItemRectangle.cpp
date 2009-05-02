/*
 *  FaceItemRectangle.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 2/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "FaceItemRectangle.h"

FaceItemRectangle :: FaceItemRectangle ()
{
	
}

FaceItemRectangle :: ~FaceItemRectangle( )
{
	/*  clean up */
//	delete this->x;
//	delete this->y;
//	delete this->w;
//	delete this->h;
//	delete this->cx;
//	delete this->cy;
}

void FaceItemRectangle :: set( float x, float y, float w, float h, float cx, float cy )
{
	this->x		= x;
	this->y		= y;
	this->w		= w;
	this->h		= h;
	this->cx	= cx;
	this->cy	= cy;
}