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
	progressStep	= 0.05;
	curveWidth		= 0.05;
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

void CurveHop :: reset()
{
	progressIn	= 0;
	progressOut	= 0;
	
	playedIn	= false;
	playedOut	= false;
}

void CurveHop :: setProgressIn( float value )
{
	progressIn = value;
}

void CurveHop :: setProgressOut( float value )
{
	progressOut = value;
}

void CurveHop :: setProgressStep( float value )
{
	progressStep = value;
}

void CurveHop :: setCurveWidth( float value )
{
	curveWidth = value;
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

bool CurveHop :: isPlayedIn()
{
	return playedIn;
}

bool CurveHop :: isPlayedOut()
{
	return playedOut;
}

void CurveHop :: draw ()
{
	ofxVec3f prev;
	ofxVec3f next;
	ofxVec3f up;
	ofxVec3f right;
	ofxVec3f look;
	
	int j1	= (int)( bezierDetail * progressIn );
	int j2	= (int)( bezierDetail * progressOut );
	int n	= j2 - j1 + 1;
	
	GLfloat *curveLineStrip	= new GLfloat[ n * 3 ];
	GLfloat	*curveColor		= new GLfloat[ n * 4 ];
	
	GLfloat *curveTriangles	= new GLfloat[ n * 6 ];
	GLfloat *curveTrigColor	= new GLfloat[ n * 8 ];
	
	for( int j=j1; j<=j2; j++ )									// draw bezier connections.
	{
		int k = j - j1;
		
		float dt = j/(float) bezierDetail;						// percent of bezier.
		
		if( j == j1 )
		{
			prev = bernstein( dt, bezierPoints );			// generate new point.
			
			// lines.
			curveLineStrip[ k * 3 + 0 ] = prev.x;
			curveLineStrip[ k * 3 + 1 ] = prev.y;
			curveLineStrip[ k * 3 + 2 ] = prev.z;
			
			curveColor[ k * 4 + 0 ]	= 1.0f;
			curveColor[ k * 4 + 1 ]	= 1.0f;
			curveColor[ k * 4 + 2 ]	= 1.0f;
			curveColor[ k * 4 + 3 ]	= 1.0f;
			
			// triangles.
			curveTriangles[ k * 6 + 0 ]  = prev.x;
			curveTriangles[ k * 6 + 1 ]  = prev.y;
			curveTriangles[ k * 6 + 2 ]  = prev.z;
			curveTriangles[ k * 6 + 3 ]  = prev.x;
			curveTriangles[ k * 6 + 4 ]  = prev.y;
			curveTriangles[ k * 6 + 5 ]  = prev.z;
			
			curveTrigColor[ k * 8 + 0 ]  = 1.0;
			curveTrigColor[ k * 8 + 1 ]  = 1.0;
			curveTrigColor[ k * 8 + 2 ]  = 1.0;
			curveTrigColor[ k * 8 + 3 ]  = 1.0;
			curveTrigColor[ k * 8 + 4 ]  = 1.0;
			curveTrigColor[ k * 8 + 5 ]  = 1.0;
			curveTrigColor[ k * 8 + 6 ]  = 1.0;
			curveTrigColor[ k * 8 + 7 ]  = 1.0;
		}
		else
		{
			next = bernstein( dt, bezierPoints );			// generate new point.
			
			// lines.
			curveLineStrip[ k * 3 + 0 ] = next.x;
			curveLineStrip[ k * 3 + 1 ] = next.y;
			curveLineStrip[ k * 3 + 2 ] = next.z;
			
			curveColor[ k * 4 + 0 ]	= 1.0f;
			curveColor[ k * 4 + 1 ]	= 1.0f;
			curveColor[ k * 4 + 2 ]	= 1.0f;
			curveColor[ k * 4 + 3 ]	= 1.0f;
			
			up		= next - prev;
			right	= up.getCrossed( prev ).normalize();
			look	= up.getCrossed( right ).normalize();
			right	= up.getCrossed( look ).normalize();

			float ss	= sin( dt * PI ) * curveWidth;			// sine width sale.
			float xOff	= right.x * ss;
			float yOff	= right.y * ss;
			float zOff	= right.z * ss;
			
			// triangles.
			curveTriangles[ k * 6 + 0 ] = next.x - xOff;
			curveTriangles[ k * 6 + 1 ] = next.y - yOff;
			curveTriangles[ k * 6 + 2 ] = next.z - zOff;
			curveTriangles[ k * 6 + 3 ] = next.x + xOff;
			curveTriangles[ k * 6 + 4 ]	= next.y + yOff;
			curveTriangles[ k * 6 + 5 ] = next.z + zOff;
			
			curveTrigColor[ k * 8 + 0 ] = 1.0;
			curveTrigColor[ k * 8 + 1 ] = 1.0;
			curveTrigColor[ k * 8 + 2 ] = 1.0;
			curveTrigColor[ k * 8 + 3 ] = 1.0;
			curveTrigColor[ k * 8 + 4 ] = 1.0;
			curveTrigColor[ k * 8 + 5 ] = 1.0;
			curveTrigColor[ k * 8 + 6 ] = 1.0;
			curveTrigColor[ k * 8 + 7 ] = 1.0;
			
			prev = next;
		}
	}
	
	drawCurveLineStrip( curveLineStrip, curveColor, n );
	drawCurveTriangleStrip( curveTriangles, curveTrigColor, n * 2 );
	
	delete[] curveLineStrip;
	delete[] curveColor;
	delete[] curveTriangles;
	delete[] curveTrigColor;
}

void CurveHop :: drawCurveLineStrip( GLfloat *vertex, GLfloat *color, int length )
{
	glVertexPointer( 3, GL_FLOAT, 0, &vertex[ 0 ] );
	glColorPointer( 4, GL_FLOAT, 0, &color[ 0 ] );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glDrawArrays( GL_LINE_STRIP, 0, length );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}

void CurveHop :: drawCurveTriangleStrip( GLfloat *vertex, GLfloat *color, int length )
{
	glVertexPointer( 3, GL_FLOAT, 0, &vertex[ 0 ] );
	glColorPointer( 4, GL_FLOAT, 0, &color[ 0 ] );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glDrawArrays( GL_TRIANGLE_STRIP, 0, length );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
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
