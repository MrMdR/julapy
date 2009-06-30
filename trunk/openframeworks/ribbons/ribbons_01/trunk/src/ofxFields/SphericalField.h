/*
 *  SphericalField.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 20/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxVectorMath.h"

class SphericalField
{

public :	
	
	SphericalField();
	~SphericalField();
	
	void setCenter( float x, float y, float z );
	void setRadius( float r );
	ofxVec3f getNormalisedForce( float x, float y, float z );
	
private :
	
	ofxVec3f	center;
	float		radius;
	
};