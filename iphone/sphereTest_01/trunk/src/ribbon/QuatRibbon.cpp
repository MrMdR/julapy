/*
 *  QuatRibbon.cpp
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 28/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "QuatRibbon.h"

QuatRibbon :: QuatRibbon()
{
	//
}

QuatRibbon :: ~QuatRibbon()
{
	
}

void QuatRibbon :: calcLine()
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
	
	ofxVec3f prev;
	ofxVec3f next;
	ofxVec3f up;
	ofxVec3f right;
	ofxVec3f look;
	
	float *m = new float[ 16 ];
	
	for( int i=0; i<detail; i++ )
	{
		float p = i / (float)( detail - 1 );
		geodesicSlerp( m, lat0, lon0, lat1, lon1, p );
		
		next.set( -m[ 8 ], -m[ 9 ], -m[ 10 ] );
		next *= ( 1 + curveHeight * sin( p * PI ) );
		
		lineStrip[ i * 3 + 0 ] = next.x;
		lineStrip[ i * 3 + 1 ] = next.y;
		lineStrip[ i * 3 + 2 ] = next.z;
		
		lineColor[ i * 4 + 0 ] = 1.0;
		lineColor[ i * 4 + 1 ] = 1.0;
		lineColor[ i * 4 + 2 ] = 1.0;
		lineColor[ i * 4 + 3 ] = 0.3;
		
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
	
	delete[] m;
}

void QuatRibbon :: rotateToSpherePoint ( float *m, float lat, float lon )
{
	ofxVec3f cp;
	
	cp = sphericalToCartesian( lat, lon, 1 );
	cp.normalize();
	
	m[0]	= 0;
	m[1]	= 0;
	m[2]	= 0;
	m[3]	= 0;
	m[4]	= 0;
	m[5]	= 0;
	m[6]	= 0;
	m[7]	= 0;
	m[8]	= -cp.x;
	m[9]	= -cp.y;
	m[10]	= -cp.z;
	m[11]	= 0;
	m[12]	= 0;
	m[13]	= 0;
	m[14]	= 0;
	m[15]	= 1;
}

void QuatRibbon :: geodesicSlerp( float *m, float lat0, float lon0, float lat1, float lon1, float dt, bool ignoreDirection )
{
	ofxQuat q0, q1, qs;
	
	float *m0 = new float[ 16 ];
	float *m1 = new float[ 16 ];
	
	if( ignoreDirection )
	{
		rotateToSpherePoint( m0, lat0, lon0 );
		rotateToSpherePoint( m1, lat1, lon1 );
	}
	else
	{
		rotateToSphereSurface( m0, lat0, lon0 );
		rotateToSphereSurface( m1, lat1, lon1 );
	}
	
	ofxMatrix4x4 mat0;
	ofxMatrix4x4 mat1;
	
	mat0._mat[ 0 ][ 0 ] = m0[ 0 ];
	mat0._mat[ 0 ][ 1 ] = m0[ 1 ];
	mat0._mat[ 0 ][ 2 ] = m0[ 2 ];
	mat0._mat[ 0 ][ 3 ] = m0[ 3 ];
	mat0._mat[ 1 ][ 0 ] = m0[ 4 ];
	mat0._mat[ 1 ][ 1 ] = m0[ 5 ];
	mat0._mat[ 1 ][ 2 ] = m0[ 6 ];
	mat0._mat[ 1 ][ 3 ] = m0[ 7 ];
	mat0._mat[ 2 ][ 0 ] = m0[ 8 ];
	mat0._mat[ 2 ][ 1 ] = m0[ 9 ];
	mat0._mat[ 2 ][ 2 ] = m0[ 10 ];
	mat0._mat[ 2 ][ 3 ] = m0[ 11 ];
	mat0._mat[ 3 ][ 0 ] = m0[ 12 ];
	mat0._mat[ 3 ][ 1 ] = m0[ 13 ];
	mat0._mat[ 3 ][ 2 ] = m0[ 14 ];
	mat0._mat[ 3 ][ 3 ] = m0[ 15 ];
	
	mat1._mat[ 0 ][ 0 ] = m1[ 0 ];
	mat1._mat[ 0 ][ 1 ] = m1[ 1 ];
	mat1._mat[ 0 ][ 2 ] = m1[ 2 ];
	mat1._mat[ 0 ][ 3 ] = m1[ 3 ];
	mat1._mat[ 1 ][ 0 ] = m1[ 4 ];
	mat1._mat[ 1 ][ 1 ] = m1[ 5 ];
	mat1._mat[ 1 ][ 2 ] = m1[ 6 ];
	mat1._mat[ 1 ][ 3 ] = m1[ 7 ];
	mat1._mat[ 2 ][ 0 ] = m1[ 8 ];
	mat1._mat[ 2 ][ 1 ] = m1[ 9 ];
	mat1._mat[ 2 ][ 2 ] = m1[ 10 ];
	mat1._mat[ 2 ][ 3 ] = m1[ 11 ];
	mat1._mat[ 3 ][ 0 ] = m1[ 12 ];
	mat1._mat[ 3 ][ 1 ] = m1[ 13 ];
	mat1._mat[ 3 ][ 2 ] = m1[ 14 ];
	mat1._mat[ 3 ][ 3 ] = m1[ 15 ];
	
	q0.set( mat0 );
	q1.set( mat1 );
	
	delete[] m0;
	delete[] m1;
	
	qs.slerp( dt, q0, q1 );
	
	ofxMatrix4x4 mat2;
	qs.get( mat2 );
	
	m[ 0 ]  = mat2._mat[ 0 ][ 0 ];
	m[ 1 ]  = mat2._mat[ 0 ][ 1 ];
	m[ 2 ]  = mat2._mat[ 0 ][ 2 ];
	m[ 3 ]  = mat2._mat[ 0 ][ 3 ];
	m[ 4 ]  = mat2._mat[ 1 ][ 0 ];
	m[ 5 ]  = mat2._mat[ 1 ][ 1 ];
	m[ 6 ]  = mat2._mat[ 1 ][ 2 ];
	m[ 7 ]  = mat2._mat[ 1 ][ 3 ];
	m[ 8 ]  = mat2._mat[ 2 ][ 0 ];
	m[ 9 ]  = mat2._mat[ 2 ][ 1 ];
	m[ 10 ] = mat2._mat[ 2 ][ 2 ];
	m[ 11 ] = mat2._mat[ 2 ][ 3 ];
	m[ 12 ] = mat2._mat[ 3 ][ 0 ];
	m[ 13 ] = mat2._mat[ 3 ][ 1 ];
	m[ 14 ] = mat2._mat[ 3 ][ 2 ];
	m[ 15 ] = mat2._mat[ 3 ][ 3 ];
}
