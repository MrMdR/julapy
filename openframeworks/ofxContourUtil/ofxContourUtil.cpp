/*
 *  ofxContourUtil.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxContourUtil.h"

ofxContourUtil :: ofxContourUtil ()
{
	//
}

ofxContourUtil :: ~ofxContourUtil ()
{
	//
}

// TODO :: check all the blob is being copied over correctly, 

void ofxContourUtil :: downSampleBlobs ( vector<ofxCvBlob> &blobsIn, vector<ofxCvBlob> &blobsOut, int numOfPoints )
{
	blobsOut.clear();
	
	for( int i=0; i<blobsIn.size(); i++ )
	{
		blobsOut.push_back( ofxCvBlob() );
		
		ofxCvBlob &b1 = blobsIn[ i ];
		ofxCvBlob &b2 = blobsOut.back();
		
		int t = MIN( numOfPoints, b1.pts.size() );
		
		b2.area			= b1.area;
		b2.hole			= b1.hole;
		b2.length		= b1.length;
		b2.centroid		= b1.centroid;
		b2.boundingRect	= b1.boundingRect;
		b2.nPts			= t;
		
		for( int j=0; j<t; j++ )
		{
			float p;
			p = j / (float)t;
			
			int k;
			k = (int)( p * b1.pts.size() );
			
			b2.pts.push_back( ofPoint() );
			
			ofPoint &pt_1 = b1.pts[ k ];
			ofPoint &pt_2 = b2.pts.back();
			
			pt_2.x = pt_1.x;
			pt_2.y = pt_1.y;
		}
	}
}

void ofxContourUtil :: scaleBlobs ( vector<ofxCvBlob> &blobsIn, float scaleX, float scaleY )		// TODO :: need to scale the whole blob, not just points!
{
	if( scaleX == 1 && scaleY == 1 )
		return;
	
	for( int i=0; i<blobsIn.size(); i++ )
	{
		ofxCvBlob &blob = blobsIn[ i ];
		
		for( int j=0; j<blob.pts.size(); j++ )
		{
			blob.pts[ j ].x *= scaleX;
			blob.pts[ j ].y *= scaleY;
		}
	}
}

void ofxContourUtil :: scaleBlobsToCenter ( vector<ofxCvBlob> &blobsIn, ofPoint &center, float radiusX, float radiusY )
{
//	for( int i=0; i<blobsIn.size(); i++ )			// TODO :: started but haven't got time to finish.
//	{
//		ofxCvBlob &blob = blobsIn[ i ];
//		
//		for( int j=0; j<blob.pts.size(); j++ )
//		{
//			ofPoint &p = blob.pts[ j ];
//			
//			float d1	= ofDist( p.x, p.y, center.x, center.y );
//			float a1	= (float)( atan2( p.x * center.y - p.y * center.x, p.x * center.x + p.y * center.y ) );
//			
//			float dx	= radiusX * sin( angl );
//			float dy	= radiusY * cos( angl );
//			float d2	= ofDist( 0, 0, dx, dy );
//			
//			float r		= 1 - ( d1 / d2 );
//		}
//	}
}

void ofxContourUtil :: translateBlobs ( vector<ofxCvBlob> &blobsIn, float x, float y )
{
	if( x == 0 && y == 0 )
		return;
	
	for( int i=0; i<blobsIn.size(); i++ )
	{
		ofxCvBlob &blob = blobsIn[ i ];
		
		for( int j=0; j<blob.pts.size(); j++ )
		{
			blob.pts[ j ].x += x;
			blob.pts[ j ].y += y;
		}
	}
}
