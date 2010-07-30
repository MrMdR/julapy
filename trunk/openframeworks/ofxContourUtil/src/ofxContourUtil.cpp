/*
 *  ofxContourUtil.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 17/05/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxContourUtil.h"

//////////////////////////////////////////
//	SMOOTH.
//////////////////////////////////////////

void ofxContourUtil :: smooth ( vector<ofPoint>& contourIn, float smoothPct )
{
	vector<ofPoint> contourOut;
	
	smooth( contourIn, contourOut, smoothPct );
	
	contourIn = contourOut;								// copies vector content over.
}

void ofxContourUtil :: smooth ( vector<ofPoint>& contourIn, vector<ofPoint>& contourOut, float smoothPct )
{
	contourOut.clear();
	contourOut.assign( contourIn.size(), ofPoint() );
	
	smoothPct = MIN( MAX( smoothPct, 0 ), 1 );
	
	float invPct;
	invPct = 1.0 - smoothPct;
	
	for( int i=0; i<contourOut.size(); i++ )			// first copy the data 
	{
		contourOut[ i ] = contourIn[ i ];
	}
	
	for( int i=1; i<contourOut.size(); i++ )			// we start at 1 as we take a small pct of the prev value
	{
		contourOut[ i ] = contourOut[ i ] * smoothPct + contourOut[ i - 1 ] * invPct;
	}
}

//////////////////////////////////////////
//	SIMPLIFY.
//////////////////////////////////////////

void ofxContourUtil :: simplify( vector<ofPoint>& contourIn, float tolerance )
{
	vector<ofPoint> contourOut;
	
	simplify( contourIn, contourOut, tolerance );
	
	contourIn.clear();
	contourIn.assign( contourOut.size(), ofPoint() );
	contourIn = contourOut;								// copies vector content over.
	
	contourOut.clear();
}

void ofxContourUtil :: simplify( vector<ofPoint>& contourIn, vector<ofPoint>& contourOut, float tolerance )
{
	int length = contourIn.size();
	
	ofxPoint3f *polyLineIn	= new ofxPoint3f[ length ];		// the polyLine simplify class needs data as a vector of ofxPoint3fs 
	ofxPoint3f *polyLineOut	= new ofxPoint3f[ length ];
	
	for( int i=0; i<length; i++ )							// first we copy over the data to a 3d point array
	{						
		polyLineIn[ i ].x	= contourIn[ i ].x;
		polyLineIn[ i ].y	= contourIn[ i ].y;
		polyLineIn[ i ].z	= 0.0f;
		polyLineOut[ i ].x	= 0.0f;
		polyLineOut[ i ].y	= 0.0f;
		polyLineOut[ i ].z	= 0.0f;
	}
	
	int numPoints = poly_simplify( tolerance, polyLineIn, length, polyLineOut );
	contourOut.clear();
	contourOut.assign( numPoints, ofxPoint2f() );
	
	for(int i = 0; i < numPoints; i++)						// copy the data to our contourOut vector
	{
		contourOut[ i ].x = polyLineOut[ i ].x;
		contourOut[ i ].y = polyLineOut[ i ].y;
	}
	
	delete[] polyLineIn;
	delete[] polyLineOut;
}

//////////////////////////////////////////
//	CONVEX HULL.
//////////////////////////////////////////

void ofxContourUtil :: convexHull ( vector<ofPoint>& contourIn )
{
	vector<ofPoint> contourOut;

	convexHull( contourIn, contourOut );
	
	contourIn.clear();
	contourIn.assign( contourOut.size(), ofPoint() );
	contourIn = contourOut;								// copies vector content over.
}

void ofxContourUtil :: convexHull ( vector<ofPoint>& contourIn, vector<ofPoint>& contourOut )
{
	int numPtsIn = contourIn.size();
	
	vector<hPoint> hullPointsIn;
	hullPointsIn.clear();
	hullPointsIn.assign( numPtsIn, hPoint() );
	
	for( int i=0; i<numPtsIn; i++ )
	{
		hullPointsIn[ i ].x = contourIn[ i ].x;
		hullPointsIn[ i ].y = contourIn[ i ].y;
	}
	
	vector<hPoint> hullPointsOut = calcConvexHull( hullPointsIn );
	int numOut = hullPointsOut.size();
	
	contourOut.clear();
	contourOut.assign( numOut, ofPoint() );
	
	for( int i=0; i<numOut; i++ )
	{
		contourOut[ i ].x = hullPointsOut[ i ].x;
		contourOut[ i ].y = hullPointsOut[ i ].y;
	}
}
