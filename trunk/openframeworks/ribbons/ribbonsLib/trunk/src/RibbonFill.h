/*
 *  RibbonFill.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_FILL_H_
#define _RIBBON_FILL_H_

#include "Ribbon.h"

class RibbonFill : public Ribbon
{
		
public :
		
	RibbonFill();
	~RibbonFill();
		
	void init();
	void setLength( int length );
	void setInitPosition( float x, float y, float z );
	void setInitVelocity( float x, float y, float z );
	void setInitColor( float r, float g, float b, float a );
	void addVelocity( float x, float y, float z );
	void addColor( float r, float g, float b, float a );
	
};

#endif