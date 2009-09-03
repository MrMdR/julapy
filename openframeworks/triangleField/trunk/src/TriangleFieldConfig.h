/*
 *  TriangleFieldConfig.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TRIANGLE_FIELD_CONFIG_H_
#define _TRIANGLE_FIELD_CONFIG_H_

#include "TriangleField.h"

class TriangleFieldConfig
{

public :
	
	TriangleFieldConfig();
	~TriangleFieldConfig();
	
	virtual void init();
	virtual void update();
	virtual void setSize( int value0, int value1 );
	virtual void setAudioNorm( float value );
	virtual void copyTo( TriangleField *tfs, int tfTotal );
	virtual void copyChangesTo( TriangleField *tfs, int tfTotal );
	
	TriangleField		*fields;
	int					fieldsTotal;
	
	int		width;
	int		height;
	float	audioNorm;
	
};

#endif