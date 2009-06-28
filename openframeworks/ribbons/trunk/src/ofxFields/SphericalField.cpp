/*
 *  SphericalField.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 20/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SphericalField.h"

SphericalField :: SphericalField()
{
	setCenter( 0, 0, 0 );
	setRadius( 0 );
}

SphericalField :: ~SphericalField()
{

}

void SphericalField :: setCenter( float x, float y, float z )
{
	center.set( x, y, z );
}

void SphericalField :: setRadius( float r )
{
	radius = r;
}

ofxVec3f SphericalField :: getNormalisedForce ( float x, float y, float z )
{
	ofxVec3f loc;
	loc.set( x, y, z );
	
	ofxVec3f frc = center - loc;
	frc.limit( radius );
	frc /= radius;
	
	return frc;
}