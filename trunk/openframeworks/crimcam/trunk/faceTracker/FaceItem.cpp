/*
 *  FaceItem.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "FaceItem.h"

FaceItem :: FaceItem()
{
	faceID			= 0;
	
	isMatch			= true;
	
	isIdle			= false;
	idleCount		= 0;
	idleCountLimit	= 10;
	
	rectBufferCount	= 0;
	rectBufferMin	= 7;
	rectBufferLimit	= 20;
	rectEaseScale	= 0.7;
	
	centreDistanceLimit	= 50;
}

FaceItem :: ~FaceItem()
{
	clearRectBuffer();

	/*  clean up */
//	delete rectBufferLimit;
//	delete rectBuffer;
//	delete rect;
}

void FaceItem :: setID( int fid )
{
	faceID = fid;
}

int FaceItem :: getID ( )
{
	return faceID;
}

void FaceItem :: set( float x, float y, float w, float h, float cx, float cy )
{
	int i;
	
	for( i=0; i<rectBufferLimit; i++ )
	{
		rectBuffer.push_back( FaceItemRectangle() );
		rectBuffer.back().set( x, y, w, h, cx, cy );
	}
	
	rect.set( x, y, w, h, cx, cy );
	rectEase.set( x, y, w, h, cx, cy );
}

void FaceItem :: add( float x, float y, float w, float h, float cx, float cy )
{
	rectBuffer.erase( rectBuffer.begin() );
	rectBuffer.push_back( FaceItemRectangle( ) );
	rectBuffer.back().set( x, y, w, h, cx, cy );
	
	rect.set( x, y, w, h, cx, cy );
}

bool FaceItem :: checkFace( float x, float y, float w, float h, float cx, float cy )
{
	isMatch = checkAgainstCentreDistance( cx, cy );
	
	return isMatch;
}

bool FaceItem :: checkAgainstCentreDistance ( float cx, float cy )
{
	float dx, dy;
	float len;
	
	dx = rect.cx - cx;
	dy = rect.cy - cy;

	len = sqrt( dx * dx + dy * dy );
	
	return ( len < centreDistanceLimit );
}

bool FaceItem :: hasBeenMatched ()
{
	return isMatch;
}

void FaceItem :: update()
{
	if( isMatch )
	{
		if( rectBufferCount < rectBufferLimit )
		{
			++rectBufferCount;
		}
	}
	else
	{
		++idleCount;
		
		if( idleCount >= idleCountLimit )
		{
			isIdle = true;
		}
	}
	
	isMatch = false;
	
	easeToLastRect();
}

void FaceItem :: easeToLastRect ()
{
	rectEase.x += ( rect.x - rectEase.x ) * rectEaseScale;
	rectEase.y += ( rect.y - rectEase.y ) * rectEaseScale;
	rectEase.w += ( rect.w - rectEase.w ) * rectEaseScale;
	rectEase.h += ( rect.h - rectEase.h ) * rectEaseScale;
}

bool FaceItem :: isFaceIdle ()
{
	return isIdle;
}

bool FaceItem :: isValid()
{
	return ( rectBufferCount > rectBufferMin );
}

void FaceItem :: clearRectBuffer ()
{
	int i;
	
	for( i=0; i<rectBuffer.size(); i++ )
	{
		rectBuffer.erase( rectBuffer.begin() + i );
	}
}