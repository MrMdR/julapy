/*
 *  RibbonFillView.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 27/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_FILL_VIEW_H_
#define _RIBBON_FILL_VIEW_H_

#include "RibbonView.h"

class RibbonFillView : public RibbonView
{
	
public :
	
	RibbonFillView();
	~RibbonFillView();

	void setup( Ribbon *r );
	void draw();
	
	void useRibbonGLitch( bool b );
	void setRibbonGlitch( int *floatArray, int arrayLength );
	
	GLuint	vbo[ 2 ];

	bool	glitchItUp;
	int 	*glitchArray;
	int		glitchArrayLength;
	
};

#endif