/*
 *  ofxFlashMovieClip.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFlashMovieClip.h"

ofxFlashMovieClip :: ofxFlashMovieClip( )
{
	typeID = OFX_FLASH_MOVIE_CLIP_TYPE;
	
	frameIndex = 0;
	
	play();
}

ofxFlashMovieClip :: ~ofxFlashMovieClip()
{
	//
}

///////////////////////////////////////////////
//	CORE.
///////////////////////////////////////////////

void ofxFlashMovieClip :: setup ( int total )
{
	for( int i=0; i<total; i++ )
	{
		ofxFlashDisplayObjectContainer* frame;
		frame = new ofxFlashDisplayObjectContainer();
		frames.push_back( frame );
	}
	
	frameIndex	= 0;
	this->frame	= frames[ frameIndex ];
}

void ofxFlashMovieClip :: update ()
{
	removeFrameChildren();			// remove + add is done on every frame. not a good idea, but will go with it for now.
	
	if( bPlay )
	{
		nextFrame();
	}
	
	cout << libraryItemName << endl;
	cout << totalFrames() << endl;
	
	addFrameChildren();
}

void ofxFlashMovieClip :: draw ()
{
	//
}

///////////////////////////////////////////////
//	FRAME CHILDREN.
///////////////////////////////////////////////

void ofxFlashMovieClip :: addFrameChildren ()
{
	for( int i=0; i<frame->children.size(); i++ )
	{
		addChild( frame->children[ i ] );
	}
}

void ofxFlashMovieClip :: removeFrameChildren ()
{
	for( int i=0; i<frame->children.size(); i++ )
	{
		removeChild( frame->children[ i ] );
	}
}

///////////////////////////////////////////////
//	MOVIE CLIP METHODS.
///////////////////////////////////////////////

void ofxFlashMovieClip :: gotoAndPlay ( int frameNum )
{
	frameIndex	= frameNum - 1;
	frameIndex	= ofClamp( frameIndex, 0, totalFrames() - 1 );
	frame		= frames[ frameIndex ];
	
	play();
}

void ofxFlashMovieClip :: gotoAndStop ( int frameNum )
{
	frameIndex	= frameNum - 1;
	frameIndex	= ofClamp( frameIndex, 0, totalFrames() - 1 );
	frame		= frames[ frameIndex ];
	
	stop();
}

void ofxFlashMovieClip :: nextFrame ()
{
	if( ++frameIndex > totalFrames() - 1 )
	{
		frameIndex = 0;
	}
	
	frame = frames[ frameIndex ];
}

void ofxFlashMovieClip :: prevFrame ()
{
	if( --frameIndex < 0 )
	{
		frameIndex = totalFrames() - 1;
	}
	
	frame = frames[ frameIndex ];
}

void ofxFlashMovieClip :: play ()
{
	bPlay = true;
}

void ofxFlashMovieClip :: stop ()
{
	bPlay = false;
}

///////////////////////////////////////////////
//	GETTERS.
///////////////////////////////////////////////

int ofxFlashMovieClip :: totalFrames ()
{
	return frames.size();
}

int ofxFlashMovieClip :: currentFrame ()
{
	return frameIndex + 1;
}

///////////////////////////////////////////////
//	GETTERS FOR FRAME CONTAINER.
///////////////////////////////////////////////

ofxFlashDisplayObjectContainer* ofxFlashMovieClip :: getFrameContainer( int i )
{
	i = (int)ofClamp( i, 0, frames.size() - 1 );
	return frames[ i ];
}