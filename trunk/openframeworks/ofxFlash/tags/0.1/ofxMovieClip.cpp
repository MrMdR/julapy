/*
 *  ofxMovieClip.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxMovieClip.h"

ofxMovieClip :: ofxMovieClip ()
{
	frameCurrent	= 0;
	framesTotal		= 0;
	frameIndex		= 0;
	
	stop();
}

ofxMovieClip :: ~ofxMovieClip ()
{
	clear();
}

///////////////////////////////////////////////
//	CORE.
///////////////////////////////////////////////

void ofxMovieClip :: setup ()
{
	//
}

void ofxMovieClip :: update ()
{
	if( framesTotal == 0 )
		return;
	
	if( bPlay )
	{
		nextFrame();
	}
	
	frameCurrent = frameIndex + 1;
}

void ofxMovieClip :: draw ()
{
	if( framesTotal == 0 )
		return;
	
	frames[ frameIndex ]->draw( 0, 0 );
}

void ofxMovieClip :: clear ()
{
	frames.clear();
}

///////////////////////////////////////////////
//	FRAMES.
///////////////////////////////////////////////

void ofxMovieClip :: addFrameSequence ( string dirPath, string fileName, int numOfDigits, string fileExtension, int numOfFiles )
{
	ofImage image;
	char imagePath[ 255 ];
	
	for( int i=0; i<numOfFiles; i++ )
	{
		string str;
		str = "";
		str += "%s%s%0";
		str += ofToString( numOfDigits, 0 );
		str += "d.%s";
		
		sprintf( imagePath, str.c_str(), dirPath.c_str(), fileName.c_str(), i, fileExtension.c_str() );
		
		image.loadImage( imagePath );
		
		addFrame( image );
		
		image.clear();
	}
	
}
#ifndef TARGET_OF_IPHONE
void ofxMovieClip :: addMovie ( ofVideoPlayer& movie )
{
	// add all frames from the movie.
}
#endif

void ofxMovieClip :: addFrame ( ofImage& image )
{
	addFrame( image.getPixels(), image.width, image.height, image.type );
}

void ofxMovieClip :: addFrame ( unsigned char* pixels, int w, int h, int imageType )
{
	frames.push_back( new ofTexture() );
	ofTexture* frame = frames.back();
	
	GLint glDataType;
	if( imageType == OF_IMAGE_GRAYSCALE )
	{
		glDataType = GL_LUMINANCE;
	}
	else if( imageType == OF_IMAGE_COLOR )
	{
		glDataType = GL_RGB;
	}
	else if( imageType == OF_IMAGE_COLOR_ALPHA )
	{
		glDataType = GL_RGBA;
	}
	else
	{
		ofLog( OF_LOG_WARNING, "ofxMovieClip :: addFrame() - invalid imageType" );
		return;
	}

	frame->allocate( w, h, glDataType );
	frame->loadData( pixels, w, h, glDataType );
	
	++framesTotal;
}

///////////////////////////////////////////////
//	MOVIE CLIP METHODS.
///////////////////////////////////////////////

void ofxMovieClip :: gotoAndPlay ( int frameNum )
{
	frameIndex = frameNum - 1;
	frameIndex = ofClamp( frameIndex, 0, framesTotal - 1 );
	
	play();
}

void ofxMovieClip :: gotoAndStop ( int frameNum )
{
	frameIndex = frameNum - 1;
	frameIndex = ofClamp( frameIndex, 0, framesTotal - 1 );

	stop();
}

void ofxMovieClip :: nextFrame ()
{
	if( ++frameIndex > framesTotal - 1 )
	{
		frameIndex = 0;
	}
}

void ofxMovieClip :: prevFrame ()
{
	if( --frameIndex < 0 )
	{
		frameIndex = framesTotal - 1;
	}
}

void ofxMovieClip :: play ()
{
	bPlay = true;
}

void ofxMovieClip :: stop ()
{
	bPlay = false;
}

///////////////////////////////////////////////
//	GETTERS.
///////////////////////////////////////////////

vector<ofTexture*>& ofxMovieClip :: getFrames ()
{
	return frames;
}

const int& ofxMovieClip :: totalFrames ()
{
	return framesTotal;
}

const int& ofxMovieClip :: currentFrame ()
{
	return frameCurrent;
}

