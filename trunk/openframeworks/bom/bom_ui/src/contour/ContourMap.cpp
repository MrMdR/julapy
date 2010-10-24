/*
 *  ContourMap.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 24/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ContourMap.h"

ContourMap :: ContourMap( Contour* contour )
{
	this->contour = contour;
	
	setup();
}

ContourMap :: ~ContourMap()
{

}

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void ContourMap :: setup()
{
	bSmoothing		= false;
	bDrawPoints		= false;
	bDrawLines		= false;
	bDrawCurves		= false;
	bDrawColor		= true;
	
	ausStroke	= (ofImage*)ofxAssets :: getInstance()->getAsset( "aus_stroke_757x590" );
	ausCover	= (ofImage*)ofxAssets :: getInstance()->getAsset( "aus_cover_1280x720"  );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void ContourMap :: update()
{
	//
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void ContourMap :: draw ()
{
	if( bDrawColor )
	{
		ofColor* c;
		c = contour->bandBaseColor;
		
		ofFill();
		ofSetColor( c->r, c->g, c->b, c->a );
		ofRect( contour->largeRect.x, contour->largeRect.y, contour->largeRect.width, contour->largeRect.height );
		
		drawContourLines( contour->contourBlobsScaled, true );
		
		ofNoFill();
		ofSetColor( 0, 0, 0 );
		drawContourLines( contour->contourBlobsScaled, false );
	}
	
	if( bDrawLines )
	{
		ofNoFill();
		ofSetColor( 0xFFFFFF );
		drawContourLines( contour->contourBlobsScaled );
	}
	
	if( bDrawCurves )
	{
		ofNoFill();
		ofSetColor( 0xFFFFFF );
		drawContourCurveLines( contour->contourBlobsScaled );
	}
	
	if( bDrawPoints )
	{
		ofFill();
		ofSetColor( 0xFF0000 );
		drawContourPoints( contour->contourBlobsScaled );
	}
	
	//-- aus map.
	
	ofEnableAlphaBlending();

	ofSetColor( 0xAAAAAA );
	ausCover->draw( 0, 0 );
	
	ofSetColor( 0xFFFFFF );
	ausStroke->draw( contour->largeRect.x, contour->largeRect.y );
	
	ofDisableAlphaBlending();
}

void ContourMap :: drawContourPoints ( vector<Blob>& blobs )
{
	for( int i=0; i<blobs.size(); i++ )
	{
		int t = blobs[ i ].pts.size();
		
		for( int j=0; j<t+1; j++ )		// extra points to close each polygon
		{
			int k = j % t;
			int r = 1;
			
			ofRect( blobs[ i ].pts[ k ].x - r, blobs[ i ].pts[ k ].y - r, r * 2, r * 2 );
		}
	}
}

void ContourMap :: drawContourLines ( vector<Blob>& blobs, bool useBlobColor )
{
	int layer = -1;
	
	for( int i=0; i<blobs.size(); i++ )
	{
		if( useBlobColor )
		{
			ofColor& c = blobs[ i ].color;
			ofSetColor( c.r, c.g, c.b );
		}
		
		int t = blobs[ i ].pts.size();
		
		if( layer != blobs[ i ].layer )				// new layer - start new shape.
		{
			layer = blobs[ i ].layer;
			
			ofBeginShape();
		}
		
		for( int j=0; j<t+1; j++ )					// extra points to close each polygon
		{
			int k = j % t;
			
			ofVertex( blobs[ i ].pts[ k ].x, blobs[ i ].pts[ k ].y );
		}
		
		if( i < blobs.size() - 1 )
		{
			if( layer != blobs[ i + 1 ].layer )		// next layer is new.
			{
				ofEndShape();
			}
			else
			{
				ofNextContour( true );				// same layer - do next contour.
			}
		}
		else										// last blob - end shape.
		{
			ofEndShape();
		}
	}
}

void ContourMap :: drawContourCurveLines ( vector<Blob>& blobs, bool useBlobColor )
{
	for( int i=0; i<blobs.size(); i++ )
	{
		if( useBlobColor )
		{
			ofColor& c = blobs[ i ].color;
			ofSetColor( c.r, c.g, c.b );
		}
		
		int t = blobs[ i ].pts.size();
		
		ofBeginShape();
		for( int j=0; j<t+3; j++ )		// extra points to close each polygon
		{
			int k = j % t;
			
			ofCurveVertex( blobs[ i ].pts[ k ].x, blobs[ i ].pts[ k ].y );
		}
		ofEndShape();
	}
}