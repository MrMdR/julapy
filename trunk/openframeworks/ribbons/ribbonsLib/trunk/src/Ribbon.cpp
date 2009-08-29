/*
 *  Ribbon.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 26/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Ribbon.h"

Ribbon :: Ribbon()
{
	//
}

Ribbon :: ~Ribbon()
{

}

void Ribbon :: init()
{
	setLength( RIBBON_DEFAULT_LENGTH );
	setWidth( RIBBON_DEFAULT_WIDTH );
	
	setInitPosition( 0, 0, 0 );
	setInitVelocity( 0, 0, 0 );
	setInitColor( 0, 0, 0, 1 );
	
	trailCount	= 1;
	
	upAxis.set( 0, 1, 0 );
	upAxisRot = 5;
}

void Ribbon :: setLength( int length )
{
	trailLength = length + 1;		// +1 because there needs to be one more point at the end to close off the ribbon.
	
	delete[] trailPosition;
	delete[] trailDirection;
	
	trailPosition	= new float[ trailLength * 3 ];
	trailDirection	= new float[ trailLength * 3 ];
}

void Ribbon :: setInitPosition( float x, float y, float z )
{
	position[ 0 ] = trailPosition[ 0 ] = x;
	position[ 1 ] = trailPosition[ 1 ] = y;
	position[ 2 ] = trailPosition[ 2 ] = z;
	
	trailPosition[ 0 ] = position[ 0 ];
	trailPosition[ 1 ] = position[ 1 ];
	trailPosition[ 2 ] = position[ 2 ];
	
	trailDirection[ 0 ] = 0;
	trailDirection[ 1 ] = 0;
	trailDirection[ 2 ] = 0;
}

void Ribbon :: setInitVelocity( float x, float y, float z )
{
	velocity[ 0 ] = x;
	velocity[ 1 ] = y;
	velocity[ 2 ] = z;
}

void Ribbon :: setInitColor( float r, float g, float b, float a )
{
	//
}

void Ribbon :: addVelocity( float x, float y, float z )
{
	velocity[ 0 ] = x;
	velocity[ 1 ] = y;
	velocity[ 2 ] = z;
	
	position[ 0 ] += velocity[ 0 ];
	position[ 1 ] += velocity[ 1 ];
	position[ 2 ] += velocity[ 2 ];
	
	memmove( trailPosition + 3, trailPosition, 3 * trailCount * sizeof(float) );
	
	trailPosition[ 0 ] = position[ 0 ];
	trailPosition[ 1 ] = position[ 1 ];
	trailPosition[ 2 ] = position[ 2 ];
	
	memmove( trailDirection + 3, trailDirection, 3 * trailCount * sizeof(float) );
	memmove( trailVerticies + 6, trailVerticies, 6 * trailCount * sizeof(float) );
	
	int m = 0;
	int n = 1;
	
	float t0x = trailPosition[ m * 3 + 0 ];		// xyz position of 1st trail point.
	float t0y = trailPosition[ m * 3 + 1 ];
	float t0z = trailPosition[ m * 3 + 2 ];
	
	float t1x = trailPosition[ n * 3 + 0 ];		// xyz position of 2nd trail point.
	float t1y = trailPosition[ n * 3 + 1 ];
	float t1z = trailPosition[ n * 3 + 2 ];
	
	ofxVec3f t0 = ofxVec3f( t0x, t0y, t0z );	// position vector of 1st trail point.
	ofxVec3f t1 = ofxVec3f( t1x, t1y, t1z );	// position vector of 2nd trail point.
	
	ofxVec3f v1 = t0 - t1;
	v1.normalize();
	ofxVec3f ya = ofxVec3f( upAxis );
	ofxVec3f v2 = ya.cross( v1 );
	ofxVec3f v3 = v1.cross( v2 ).normalize();
	
	trailDirection[ 0 ] = v3.x;
	trailDirection[ 1 ] = v3.y;
	trailDirection[ 2 ] = v3.z;
}

void Ribbon :: addColor( float r, float g, float b, float a )
{
	//
}

void Ribbon :: rotate()
{
	upAxis.rotate( upAxisRot, ofxVec3f( 1, 0, 0 ) );
}

void Ribbon :: update()
{
	if( trailCount < trailLength )
		trailCount += 1;
}

void Ribbon :: setWidth( float value )
{
	trailWidth = value;
}