/*
 *  FaceTracker.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "FaceTracker.h"

FaceTracker :: FaceTracker()
{
	faceIteratorIndex	= 0;
	
	faceIDCounter		= 10;
	faceIDCounterLimit	= 99;
}

FaceTracker :: ~FaceTracker()
{
	
}

void FaceTracker :: init( int w, int h )
{
	haarFinder.setup( "haarXML/haarcascade_frontalface_default.xml" );
}

void FaceTracker :: findFaces( ofxCvGrayscaleImage *image )
{
	int i, j;
	float x, y, w, h, cx, cy;
	bool isMatch;
	
	haarFinder.findHaarObjects( *image, 10, 99999999, 10 );
	
//	clearFaces();
	
	for( i=0; i<haarFinder.blobs.size(); i++ )
	{
		x	= haarFinder.blobs[i].boundingRect.x;
		y	= haarFinder.blobs[i].boundingRect.y;
		w	= haarFinder.blobs[i].boundingRect.width;
		h	= haarFinder.blobs[i].boundingRect.height;
		cx	= haarFinder.blobs[i].centroid.x;
		cy	= haarFinder.blobs[i].centroid.y;
		
		isMatch = false;
		
		for( j=0; j<faces.size(); j++ )
		{
			if( isMatch )
			{
				break;
			}
				
			if( !faces[ j ].hasBeenMatched() )
			{
				isMatch = faces[ j ].checkFace( x, y, w, h, cx, cy );
				
				if( isMatch )
				{
					faces[ j ].add( x, y, w, h, cx, cy );
				}
			}
		}
		
		if( !isMatch )
		{
			faces.push_back( FaceItem() );
			faces.back().set( x, y, w, h, cx, cy );
			faces.back().setID( faceIDCounter );
		}
		
		++faceIDCounter;
		if( faceIDCounter > faceIDCounterLimit )
		{
			faceIDCounter = 10;
		}
	}
	
	for( j=0; j<faces.size(); j++ )
	{
		faces[ j ].update();
		
		if( faces[ j ].isFaceIdle() )
		{
			faces.erase( faces.begin() + j );
		}
	}
}

bool FaceTracker :: hasNextFace ()
{
	bool b;
	
	b = true;
	
	while( faceIteratorIndex < faces.size() && !faces[ faceIteratorIndex ].isValid() )
	{
		++faceIteratorIndex;
	}
	
	b = ( faceIteratorIndex < faces.size() );
	
	if( !b )
	{
		faceIteratorIndex = 0;
	}
	
	return b;
}

int FaceTracker :: getFaceID ()
{
	return faces[ faceIteratorIndex ].getID();
}

void FaceTracker :: getFaceProperties( float *x, float *y, float *w, float *h, float *cx, float *cy )
{
//	*x	= faces[ faceIteratorIndex ].rect.x;
//	*y	= faces[ faceIteratorIndex ].rect.y;
//	*w	= faces[ faceIteratorIndex ].rect.w;
//	*h	= faces[ faceIteratorIndex ].rect.h;
//	*cx	= faces[ faceIteratorIndex ].rect.cx;
//	*cy	= faces[ faceIteratorIndex ].rect.cy;

	*x	= faces[ faceIteratorIndex ].rectEase.x;
	*y	= faces[ faceIteratorIndex ].rectEase.y;
	*w	= faces[ faceIteratorIndex ].rectEase.w;
	*h	= faces[ faceIteratorIndex ].rectEase.h;
	*cx	= faces[ faceIteratorIndex ].rectEase.cx;
	*cy	= faces[ faceIteratorIndex ].rectEase.cy;
	
	++faceIteratorIndex;
}

void FaceTracker :: clearFaces ()
{
	int i;
	
	for( i=0; i<faces.size(); i++ )
	{
		faces.erase( faces.begin() + i );
	}
}
