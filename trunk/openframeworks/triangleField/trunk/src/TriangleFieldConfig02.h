/*
 *  TriangleFieldConfig02.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TRIANGLE_FIELD_CONFIG_02_H_
#define _TRIANGLE_FIELD_CONFIG_02_H_

#include "TriangleFieldConfig.h"

class TriangleFieldConfig02 : public TriangleFieldConfig
{
		
public :
		
	TriangleFieldConfig02();
	~TriangleFieldConfig02();
		
	void init();
	void update();
	void setSize( float w, float h );
	void copyChangesTo( TriangleField *tfs, int tfTotal );
		
};

#endif