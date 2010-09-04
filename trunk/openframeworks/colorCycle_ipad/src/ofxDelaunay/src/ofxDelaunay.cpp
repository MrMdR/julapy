/*
 *  ofxDelaunay.cpp
 *  openFrameworks
 *
 *  Created by Pat Long on 29/10/09.
 *  Copyright 2009 Tangible Interaction. All rights reserved.
 *
 *  Some parts based on demo code by Gilles Dumoulin. 
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *
 */
#include "ofxDelaunay.h"

ofxDelaunay :: ofxDelaunay()
{
	//
}

ofxDelaunay :: ~ofxDelaunay()
{
	//
}

int ofxDelaunay :: addPoint( const ofPoint& point )
{
	int i = addPoint( point.x, point.y, point.z );
	return i;
}

int ofxDelaunay :: addPoint( float x, float y, float z )
{
	points.push_back( XYZ() );
	points.back().x = x;
	points.back().y = y;
	points.back().z = z;
	
	return points.size();
}

int ofxDelaunay :: addPoints ( const vector<ofPoint>& pts )
{
	for( int i; i<pts.size(); i++ )
	{
		points.push_back( XYZ() );
		points.back().x = pts[ i ].x;
		points.back().y = pts[ i ].y;
		points.back().z = pts[ i ].z;
	}
	
	return points.size();
}

int ofxDelaunay :: triangulate()
{
	vector<ITRIANGLE> trianglesOut;
	trianglesOut.assign( points.size() * 3, ITRIANGLE() );
	
	qsort( &points[ 0 ], points.size(), sizeof( XYZ ), XYZCompare );
	
	int numOfTriangles;
	numOfTriangles = 0;
	
	Triangulate( points.size(), &points[ 0 ], &trianglesOut[ 0 ], numOfTriangles );

	// copy triangle data to ofxDelaunayTriangle.
	
	triangles.clear();
	
	for( int i=0; i<numOfTriangles; i++ )
	{
		triangles.push_back( ofxDelaunayTriangle() );
		ofxDelaunayTriangle& triangle = triangles.back();
		
		int p1, p2, p3;
		p1 = trianglesOut[ i ].p1;
		p2 = trianglesOut[ i ].p2;
		p3 = trianglesOut[ i ].p3;
		
		triangle.pointIndex[ 0 ] = p1;
		triangle.pointIndex[ 1 ] = p2;
		triangle.pointIndex[ 2 ] = p3;
		
		triangle.points[ 0 ].set( points[ p1 ].x, points[ p1 ].y, points[ p1 ].z );
		triangle.points[ 1 ].set( points[ p2 ].x, points[ p2 ].y, points[ p2 ].z );
		triangle.points[ 2 ].set( points[ p3 ].x, points[ p3 ].y, points[ p3 ].z );
	}
	
	trianglesOut.clear();
	
	return numOfTriangles;
}

void ofxDelaunay :: reset()
{
	points.clear();
	triangles.clear();
}

void ofxDelaunay :: draw ()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		ofxDelaunayTriangle& triangle = triangles[ i ];
		
		ofTriangle
		(
			triangle.points[ 0 ].x,
			triangle.points[ 0 ].y,
			triangle.points[ 1 ].x,
			triangle.points[ 1 ].y,
			triangle.points[ 2 ].x,
			triangle.points[ 2 ].y
		);
	}
}
