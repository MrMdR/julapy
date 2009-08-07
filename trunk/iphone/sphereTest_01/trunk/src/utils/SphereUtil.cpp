/*
 *  SphereUtil.cpp
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 21/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SphereUtil.h"

ofxVec3f sphericalToCartesian( float lat, float lon, float radius )
{
//	ofxVec3f result;
//	
//	lat	*= -1;		// inverse latitude.
//	lat += 90;		// latitude offset to match geometry of the sphere.
//	lon *= -1;		// inverse longitude.
//	lon -= 90;		// longitude offset to match geometry of the sphere.
//	
//	lat *= DEG_TO_RAD;
//	lon *= DEG_TO_RAD;
//	
//	result.x = radius * sin( lat ) * cos( lon );
//	result.y = radius * sin( lat ) * sin( lon );
//	result.z = radius * cos( lat );
//	
//	return result;
	
	ofxVec3f p;
	
	lat *= DEG_TO_RAD;
	lon *= DEG_TO_RAD;
	
	lat *= -1;		// latitude is inversed as normally y-axis points up but in openframeworks it points down.
	
	p.x = radius * cos( lat ) * sin( lon );
	p.y = radius * sin( lat );
	p.z = radius * cos( lat ) * cos( lon );
	
	return p;
	
}

ofxVec3f bernstein( float u, float *bezPoints )
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

void rotateToSphereSurface( float *m, float lat, float lon )
{
	ofxVec3f cp;
	ofxVec3f xz;
	ofxVec3f yz;
	
	cp = sphericalToCartesian( lat, lon, 1 );
	cp.normalize();
	
	xz = ofxVec3f( cp.x, 0, cp.z );
	xz.rotate( 90, ofxVec3f( 0, 1, 0 ) );
	xz.normalize();
	
	yz = xz.getCrossed( cp );
	yz.normalize();
	
	m[0]	= xz.x;
	m[1]	= xz.y;
	m[2]	= xz.z;
	m[3]	= 0;
	m[4]	= yz.x;
	m[5]	= yz.y;
	m[6]	= yz.z;
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

void rotateToSphereSurface( float *m, float x, float y, float z )
{
	ofxVec3f cp;
	ofxVec3f xz;
	ofxVec3f yz;
	
	cp.set( x, y, z );
	cp.normalize();
	
	xz = ofxVec3f( cp.x, 0, cp.z );
	xz.rotate( 90, ofxVec3f( 0, 1, 0 ) );
	xz.normalize();
	
	yz = xz.getCrossed( cp );
	yz.normalize();
	
	m[0]	= xz.x;
	m[1]	= xz.y;
	m[2]	= xz.z;
	m[3]	= 0;
	m[4]	= yz.x;
	m[5]	= yz.y;
	m[6]	= yz.z;
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

