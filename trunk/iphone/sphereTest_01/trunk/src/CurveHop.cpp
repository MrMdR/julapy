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
	progressIn		= 0;
	progressOut		= 0;
	progressStep	= 0.05f;
	bezierDetail	= 50;
	playedIn		= false;
	playedOut		= false;
}

CurveHop :: ~CurveHop()
{
	//
}

void CurveHop :: set( float *bezPoints )
{
	for( int i=0; i<12; i++ )
	{
		bezierPoints[ i ] = bezPoints[ i ];
	}
}

void CurveHop :: playInStep ()
{
	progressOut += progressStep;
	
	if( progressOut >= 1 )
	{
		playedIn = true;
	}
}

void CurveHop :: playOutStep ()
{
	progressIn += progressStep;
	if( progressIn >= 1 )
	{
		playedOut = true;
	}
}

void CurveHop :: draw ()
{
	int j, j1, j2;
	
	ofxVec3f prev;
	ofxVec3f next;
	ofxVec3f up;
	ofxVec3f right;
	ofxVec3f look;
	
//	glBegin( GL_QUAD_STRIP );
	
	float dt;
	
	j1 = (int)( bezierDetail * progressIn );
	j2 = (int)( bezierDetail * progressOut );
	for( j=j1; j<=j2; j++)									// draw bezier connections.
	{									
		dt = (float)j/(float) bezierDetail;					// percent of bezier.
		
		if( j == j1 )
		{
			prev = bernstein( dt, bezierPoints );			// generate new point.
		}
		else
		{
			next = bernstein( dt, bezierPoints );			// generate new point.
			
			up		= prev - next;
			right	= up.cross( prev ).normalize();
			look	= up.cross( right ).normalize();
			right	= up.cross( look ).normalize();
			
			float ss	= sin( dt * PI );					// sine width sale.
			float xOff	= right.x * 6 * ss;
			float yOff	= right.y * 6 * ss;
			float zOff	= right.z * 6 * ss;
			
//			gl.glColor4f( 1, 0.75f * ss, 0.23f * ss, max( 0.2f, ss ) );
//			gl.glNormal3f( look.x, look.y, look.z ); 						// not sure if this is right.
//			gl.glVertex3f( prev.x - xOff, prev.y - yOff, prev.z - zOff );
//			gl.glVertex3f( prev.x + xOff, prev.y + yOff, prev.z + zOff );
			
			/* draw line */
			//gl.glVertex3d( nextPoint.x, nextPoint.y, nextPoint.z );	// draw vertex for point.
			
			prev = next;
		}
	}
//	glEnd();
}


ofxVec3f CurveHop :: bernstein( float u, float *bezPoints )
{
	ofxVec3f a = ofxVec3f( bezPoints[ 0 ], bezPoints[ 1 ], bezPoints[ 2 ] );
	ofxVec3f b = ofxVec3f( bezPoints[ 3 ], bezPoints[ 4 ], bezPoints[ 5 ] );
	ofxVec3f c = ofxVec3f( bezPoints[ 6 ], bezPoints[ 7 ], bezPoints[ 8 ] );
	ofxVec3f d = ofxVec3f( bezPoints[ 9 ], bezPoints[ 10 ], bezPoints[ 11 ] );
	
	a *= pow( u, 3 );
	b *= ( 3 * pow( u, 2 ) * ( 1 - u ) );
	c *= ( 3 * u * pow( ( 1 - u ), 2 ) );
	d *= pow( ( 1 - u ), 3 );
	
	ofxVec3f v = a + b + c + d;
	
	return v;
	
//	Original code from a java Point3D class.
//	Point3D a = p[0].scale(Math.pow(u, 3));
//	Point3D b = p[1].scale(3 * Math.pow(u, 2) * (1 - u));
//	Point3D c = p[2].scale(3 * u * Math.pow((1 - u), 2));
//	Point3D d = p[3].scale(Math.pow((1 - u), 3));
//	
//	return a.add(b).add(c).add(d);
}
