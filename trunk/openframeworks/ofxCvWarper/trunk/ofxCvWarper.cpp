/*
 *  ofxCvWarper.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 17/10/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxCvWarper.h"

ofxCvWarper :: ofxCvWarper()
{
	anchorSize = 10;
}

ofxCvWarper :: ~ofxCvWarper()
{
	//
}

void ofxCvWarper :: setup( ofxCvImage *_srcImage, ofxCvImage *_dstImage )
{
	srcImage = _srcImage;
	dstImage = _dstImage;
	
	srcRect.width  = srcImage->width;
	srcRect.height = srcImage->height;
	
	dstRect.width  = dstImage->width;
	dstRect.height = dstImage->height;

	ofAddListener( ofEvents.mouseDragged, this, &ofxCvWarper :: onMouseDragged );
	
	reset();
}

void ofxCvWarper :: setPosition( float x, float y )
{
	pos.x = x;
	pos.y = y;
	
	reset();
}

void ofxCvWarper :: warp()
{
	dstImage->warpIntoMe( *srcImage, srcPoints, dstPoints );
}

void ofxCvWarper :: draw()
{
	glPushMatrix();
	glTranslatef( pos.x, pos.y, 0 );
	for( int i=0; i<4; i++ )
	{
		glColor3f( 0.0f, 1.0f, 0.0f );
		ofCircle( srcPoints[ i ].x, srcPoints[ i ].y, 3 );
	}
	glColor3f( 0.0f, 1.0f, 0.0f );
	
	ofLine( srcPoints[ 0 ].x, srcPoints[ 0 ].y, srcPoints[ 1 ].x, srcPoints[ 1 ].y );
	ofLine( srcPoints[ 1 ].x, srcPoints[ 1 ].y, srcPoints[ 2 ].x, srcPoints[ 2 ].y );
	ofLine( srcPoints[ 2 ].x, srcPoints[ 2 ].y, srcPoints[ 3 ].x, srcPoints[ 3 ].y );
	ofLine( srcPoints[ 3 ].x, srcPoints[ 3 ].y, srcPoints[ 0 ].x, srcPoints[ 0 ].y );
	
	glPopMatrix();
	glColor3f( 1.0f, 1.0f, 1.0f );
}

void ofxCvWarper :: reset()
{
	ofRectangle	rect;
	rect = ofxResizeUtil :: fitToSize( dstRect, srcRect );
	
	srcPoints[ 0 ].set( rect.x, rect.y, 0 );
	srcPoints[ 1 ].set( rect.x + rect.width, rect.y, 0 );
	srcPoints[ 2 ].set( rect.x + rect.width, rect.y + rect.height, 0 );
	srcPoints[ 3 ].set( rect.x, rect.y + rect.height, 0 );

	for( int i=0; i<4; i++ )
	{
		anchors[ i ].setPosAndSize
		(
			srcPoints[ i ].x + pos.x - anchorSize * 0.5,
			srcPoints[ i ].y + pos.y - anchorSize * 0.5,
			anchorSize,
			anchorSize
		);
		anchors[ i ].enableMouseEvents();
	}
	
	dstPoints[ 0 ].set( 0, 0 );
	dstPoints[ 1 ].set( dstRect.width, 0 );
	dstPoints[ 2 ].set( dstRect.width, dstRect.height );
	dstPoints[ 3 ].set( 0, dstRect.height );
}

void ofxCvWarper :: onMouseDragged( ofMouseEventArgs& mouseArgs )
{
	for( int i=0; i<4; i++ )
	{
		if( anchors[ i ].isMouseDown() )
		{
			anchors[ i ].setPos
			(
				mouseArgs.x - anchorSize * 0.5,
				mouseArgs.y - anchorSize * 0.5
			);
			
			srcPoints[ i ].set
			(
				mouseArgs.x - pos.x,
				mouseArgs.y - pos.y,
				0
			);
		}
	}
}

///////////////////////////////////////////////////
//	SAVE / LOAD
///////////////////////////////////////////////////

void ofxCvWarper :: save( string fileName )
{
	ofstream myFile;
	myFile.open( ofToDataPath( fileName ).c_str() );
	if( myFile.is_open() )
	{
		for( int i=0; i<4; i++ )
		{
			myFile << srcPoints[ i ].x << endl;
			myFile << srcPoints[ i ].y << endl;
		}
	}
	myFile.close();
}

void ofxCvWarper :: load( string fileName )
{
	ifstream myFile;
	string line;
	myFile.open( ofToDataPath( fileName ).c_str() );
	if( myFile.is_open() )
	{
		int pointIndex	= 0;
		int lineIndex	= 0;
		while( !myFile.eof() )
		{
			getline( myFile, line );
			if( lineIndex % 2 == 0)
			{
				srcPoints[ pointIndex ].x = atof( line.c_str() );
			}
			else
			{
				srcPoints[ pointIndex ].y = atof( line.c_str() );
			}
			lineIndex++;
			pointIndex = (int)( lineIndex * 0.5f );
		}
	}
	myFile.close();
}

