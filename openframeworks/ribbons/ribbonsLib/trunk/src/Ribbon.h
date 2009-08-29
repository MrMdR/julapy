/*
 *  Ribbon.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 26/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_H_
#define _RIBBON_H_

#define RIBBON_DEFAULT_LENGTH	100
#define RIBBON_DEFAULT_WIDTH	2

#include "ofxVec3f.h"

class Ribbon
{
	
public :
	
	Ribbon();
	~Ribbon();
	
	virtual void init();
	virtual void setLength( int length );
	virtual void setInitPosition( float x, float y, float z );
	virtual void setInitVelocity( float x, float y, float z );
	virtual void setInitColor( float r, float g, float b, float a );
	virtual void addVelocity( float x, float y, float z );
	virtual void addColor( float r, float g, float b, float a );
	virtual void rotate();
	virtual void update();
	
	virtual void setWidth( float value );
	
	float	position[ 3 ];
	float	velocity[ 3 ];
	float	color[ 4 ];
	
	int		trailLength;
	int		trailCount;
	float	trailWidth;
	
	float	*trailPosition;
	float	*trailDirection;
	float	*trailVerticies;
	float	*trailColor;

	ofxVec3f	upAxis;
	float		upAxisRot;

};

#endif