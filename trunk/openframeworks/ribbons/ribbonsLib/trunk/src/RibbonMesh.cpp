/*
 *  RibbonMesh.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonMesh.h"

RibbonMesh :: RibbonMesh()
{
	init();
}

RibbonMesh :: ~RibbonMesh()
{
	
}

void RibbonMesh :: setLength( int length )
{
	Ribbon :: setLength( length );
	
	delete[] trailVerticies;
	delete[] trailColor;
	
	trailVerticies	= new float[ trailLength * 3 * 2 ];
	trailColor		= new float[ trailLength * 4 * 2 ];
}

void RibbonMesh :: setInitPosition( float x, float y, float z )
{
	Ribbon :: setInitPosition( x, y, z );
	
	trailVerticies[ 0 ] = position[ 0 ];
	trailVerticies[ 1 ] = position[ 1 ];
	trailVerticies[ 2 ] = position[ 2 ];
	trailVerticies[ 3 ] = position[ 0 ];
	trailVerticies[ 4 ] = position[ 1 ];
	trailVerticies[ 5 ] = position[ 2 ];
}

void RibbonMesh :: setInitVelocity( float x, float y, float z )
{
	Ribbon :: setInitVelocity( x, y, z );
}

void RibbonMesh :: setInitColor( float r, float g, float b, float a )
{
	trailColor[ 0 ] = trailColor[ 4 ] = r;
	trailColor[ 1 ] = trailColor[ 5 ] = g;
	trailColor[ 2 ] = trailColor[ 6 ] = b;
	trailColor[ 3 ] = trailColor[ 7 ] = a;
}

void RibbonMesh :: addVelocity( float x, float y, float z )
{
	Ribbon :: addVelocity( x, y, z );
	
	float w		= trailWidth;
	float xOff	= trailDirection[ 0 ] * w;
	float yOff	= trailDirection[ 1 ] * w;
	float zOff	= trailDirection[ 2 ] * w;
	
	int m = 0;
	int n = 1;
	
	float t0x = trailPosition[ m * 3 + 0 ];		// xyz position of 1st trail point.
	float t0y = trailPosition[ m * 3 + 1 ];
	float t0z = trailPosition[ m * 3 + 2 ];
	
	float t1x = trailPosition[ n * 3 + 0 ];		// xyz position of 2nd trail point.
	float t1y = trailPosition[ n * 3 + 1 ];
	float t1z = trailPosition[ n * 3 + 2 ];
	
	trailVerticies[ 0 ] = t0x - xOff;
	trailVerticies[ 1 ] = t0y - yOff;
	trailVerticies[ 2 ] = t0z - zOff;
	trailVerticies[ 3 ] = t0x + xOff;
	trailVerticies[ 4 ] = t0y + yOff;
	trailVerticies[ 5 ] = t0z + zOff;
}

void RibbonMesh :: addColor( float r, float g, float b, float a )
{
	memmove( trailColor + 4 * 2, trailColor, 4 * 2 * trailCount * sizeof(float) );
	
	trailColor[ 0 ] = trailColor[ 4 ] = r;
	trailColor[ 1 ] = trailColor[ 5 ] = g;
	trailColor[ 2 ] = trailColor[ 6 ] = b;
	trailColor[ 3 ] = trailColor[ 7 ] = a;
}
