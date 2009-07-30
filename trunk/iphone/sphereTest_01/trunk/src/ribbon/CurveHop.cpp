/*
 *  CurveHop.cpp
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 10/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "CurveHop.h"

CurveHop :: CurveHop()
{
	//
}

CurveHop :: ~CurveHop()
{
	//
}

void CurveHop :: calcLine ()
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
	
	ofxVec3f p1 = sphericalToCartesian( lat0, lon0, 1 );
	ofxVec3f c1 = sphericalToCartesian( lat0, lon0, 1 + curveHeight );
	ofxVec3f p2 = sphericalToCartesian( lat1, lon1, 1 );
	ofxVec3f c2 = sphericalToCartesian( lat1, lon1, 1 + curveHeight );
	
	float *bezierPoints	= new float[ 12 ];
	bezierPoints[ 0 ]	= p2.x;
	bezierPoints[ 1 ]	= p2.y;
	bezierPoints[ 2 ]	= p2.z;
	
	bezierPoints[ 3 ]	= c2.x;
	bezierPoints[ 4 ]	= c2.y;
	bezierPoints[ 5 ]	= c2.z;
	
	bezierPoints[ 6 ]	= c1.x;
	bezierPoints[ 7 ]	= c1.y;
	bezierPoints[ 8 ]	= c1.z;
	
	bezierPoints[ 9 ]	= p1.x;
	bezierPoints[ 10 ]	= p1.y;
	bezierPoints[ 11 ]	= p1.z;
	
	ofxVec3f prev;
	ofxVec3f next;
	ofxVec3f up;
	ofxVec3f right;
	ofxVec3f look;
	
	for( int i=0; i<detail; i++ )
	{
		float p = i / (float)( detail - 1 );
		
		next = bernstein( p, bezierPoints );
		
		if( i == 0 )
		{
			prev = next;
		}

		lineStrip[ i * 3 + 0 ] = next.x;
		lineStrip[ i * 3 + 1 ] = next.y;
		lineStrip[ i * 3 + 2 ] = next.z;
		
		lineColor[ i * 4 + 0 ]	= 1.0f;
		lineColor[ i * 4 + 1 ]	= 1.0f;
		lineColor[ i * 4 + 2 ]	= 1.0f;
		lineColor[ i * 4 + 3 ]	= 1.0f;
		
		up		= next - prev;
		right	= up.getCrossed( prev ).normalize();
		look	= up.getCrossed( right ).normalize();
		right	= up.getCrossed( look ).normalize();
		
		float ss	= sin( p * PI ) * curveWidth;
		float xOff	= right.x * ss;
		float yOff	= right.y * ss;
		float zOff	= right.z * ss;
		
		triangleStrip[ i * 6 + 0 ] = next.x - xOff;
		triangleStrip[ i * 6 + 1 ] = next.y - yOff;
		triangleStrip[ i * 6 + 2 ] = next.z - zOff;
		triangleStrip[ i * 6 + 3 ] = next.x + xOff;
		triangleStrip[ i * 6 + 4 ] = next.y + yOff;
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
	
	delete[] bezierPoints;
}