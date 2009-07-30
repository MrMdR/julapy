/*
 *  SphereRibbon.cpp
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 21/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SphereRibbon.h"

SphereRibbon :: SphereRibbon()
{
	//
}

SphereRibbon :: ~SphereRibbon()
{

}

void SphereRibbon :: calcLine()
{
	delete[] lineStrip;
	delete[] lineColor;
	delete[] triangleStrip;
	delete[] triangleStripNormal;
	delete[] triangleStripColor;
	
	lineStrip			= new GLfloat[ detail * 3 ];
	lineColor			= new GLfloat[ detail * 4 ];
	triangleStrip		= new GLfloat[ detail * 6 ];
	triangleStripNormal	= new GLfloat[ detail * 6 ];
	triangleStripColor	= new GLfloat[ detail * 8 ];
	
	float lat0, lon0;
	float lat1, lon1;
	
	lat0 = points[ 0 ];
	lon0 = points[ 1 ];
	lat1 = points[ 2 ];
	lon1 = points[ 3 ];
	
	float latLeft;
	float latRight;
	float lonLeft;
	float lonRight;
	
	float latDist;
	float lonDist;
	
	if( lat1 < lat0 )					// latitude distance to the left.
		latLeft = lat0 - lat1;
	else
		latLeft = lat0 - lat1 + 180;

	if( lat1 > lat0 )					// latitude distance to the right.
		latRight = lat1 - lat0;
	else
		latRight = lat1 - lat0 + 180;
	
	if( lon1 < lon0 )					// longitude distance to the left.
		lonLeft = lon0 - lon1;
	else
		lonLeft = lon0 - lon1 + 360;

	if( lon1 > lon0 )					// longitude distance to the right.
		lonRight = lon1 - lon0;
	else
		lonRight = lon1 - lon0 + 360;
	
	if( latLeft < latRight )			// shortest latitude distance and direction.
		latDist = -latLeft;
	else
		latDist = latRight;
	
	if( lonLeft < lonRight )			// shortest longitude distance and direction.
		lonDist = -lonLeft;
	else
		lonDist = lonRight;
	
	ofxVec3f prev;
	ofxVec3f next;
	ofxVec3f up;
	ofxVec3f right;
	ofxVec3f look;
	
	for( int i=0; i<detail; i++ )
	{
		float p	  = i / (float)( detail - 1 );
		float lat = lat0 + latDist * p;
		float lon = lon0 + lonDist * p;
		
		if( lat > 90 )				// wrap around latitude.
			lat -= 180;
		if( lat < -90 )
			lat += 180;

		if( lon > 180 )				// wrap around longitude.
			lon -= 360;
		if( lon < -180 )
			lon += 360;
		
		float r = 1 + curveHeight * sin( p * PI );
		next = sphericalToCartesian( lat, lon, r );
		
		lineStrip[ i * 3 + 0 ] = next.x;
		lineStrip[ i * 3 + 1 ] = next.y;
		lineStrip[ i * 3 + 2 ] = next.z;
		
		lineColor[ i * 4 + 0 ] = 0;
		lineColor[ i * 4 + 1 ] = 1;
		lineColor[ i * 4 + 2 ] = 0;
		lineColor[ i * 4 + 3 ] = 1;
		
		if( i == 0 )
		{
			prev = next;
		}
		
		up		= next - prev;
		right	= up.getCrossed( prev ).normalize();
		look	= up.getCrossed( right ).normalize();
		right	= up.getCrossed( look ).normalize();
		
		float ss	= sin( p * PI ) * curveWidth;			// sine width sale.
		float xOff	= right.x * ss;
		float yOff	= right.y * ss;
		float zOff	= right.z * ss;
		
		triangleStrip[ i * 6 + 0 ] = next.x - xOff;
		triangleStrip[ i * 6 + 1 ] = next.y - yOff;
		triangleStrip[ i * 6 + 2 ] = next.z - zOff;
		triangleStrip[ i * 6 + 3 ] = next.x + xOff;
		triangleStrip[ i * 6 + 4 ]	= next.y + yOff;
		triangleStrip[ i * 6 + 5 ] = next.z + zOff;
		
		triangleStripNormal[ i * 6 + 0 ] = look.x;
		triangleStripNormal[ i * 6 + 1 ] = look.y;
		triangleStripNormal[ i * 6 + 2 ] = look.z;
		triangleStripNormal[ i * 6 + 3 ] = look.x;
		triangleStripNormal[ i * 6 + 4 ] = look.y;
		triangleStripNormal[ i * 6 + 5 ] = look.z;
		
		triangleStripColor[ i * 8 + 0 ] = 1.0;
		triangleStripColor[ i * 8 + 1 ] = 1.0;
		triangleStripColor[ i * 8 + 2 ] = 1.0;
		triangleStripColor[ i * 8 + 3 ] = 0.6;
		triangleStripColor[ i * 8 + 4 ] = 1.0;
		triangleStripColor[ i * 8 + 5 ] = 1.0;
		triangleStripColor[ i * 8 + 6 ] = 1.0;
		triangleStripColor[ i * 8 + 7 ] = 0.6;
		
		prev = next;
	}
}