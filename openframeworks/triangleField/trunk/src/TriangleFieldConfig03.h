/*
 *  TriangleFieldConfig03.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TRIANGLE_FIELD_CONFIG_03_H_
#define _TRIANGLE_FIELD_CONFIG_03_H_

#include "TriangleFieldConfig.h"

class TriangleFieldConfig03 : public TriangleFieldConfig
{
		
public :
		
	TriangleFieldConfig03();
	~TriangleFieldConfig03();
		
	void init();
	void update();
	void setSize( float w, float h );
	void copyChangesTo( TriangleField *tfs, int tfTotal );
		
};

#endif