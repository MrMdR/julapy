/*
 *  RibbonMeshView.h
 *  ribbonVBOTest
 *
 *  Created by lukasz karluk on 28/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_MESH_VIEW_H_
#define _RIBBON_MESH_VIEW_H_

#include "RibbonView.h"

class RibbonMeshView : public RibbonView
{
		
public :
		
	RibbonMeshView();
	~RibbonMeshView();
		
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