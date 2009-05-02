/*
 *  FaceItemRectangle.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 2/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

class FaceItemRectangle
{
		
public :
		
	FaceItemRectangle();
	~FaceItemRectangle();
	
	void set( float x, float y, float w, float h, float cx, float cy );
	
	float x;
	float y;
	float w;
	float h;
	float cx;
	float cy;

};