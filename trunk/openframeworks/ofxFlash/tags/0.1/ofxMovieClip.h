/*
 *  ofxMovieClip.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxSprite.h"

class ofxMovieClip : public ofxSprite
{

public :
	
	~ofxMovieClip();
	 ofxMovieClip();
	
	virtual void setup	();
	virtual void update	();
	virtual void draw	();
	virtual void clear	();
	
	virtual void addFrameSequence	( string dirPath, string fileName, int numOfDigits, string fileExtension, int numOfFiles );
#ifndef TARGET_OF_IPHONE
	virtual void addMovie			( ofVideoPlayer& movie );
#endif
	virtual void addFrame			( ofImage& image );
	virtual void addFrame			( unsigned char* pixels, int w, int h, int imageType );
	
	virtual void gotoAndPlay	( int frameNum );
	virtual void gotoAndStop	( int frameNum );
	virtual void nextFrame		();
	virtual void prevFrame		();
	virtual void play			();
	virtual void stop			();
	
	vector<ofTexture*>&			getFrames		();
	const int&					totalFrames		();
	const int&					currentFrame	();
	
private :

	vector<ofTexture*>	frames;
	int					frameIndex;
	int					frameCurrent;
	int					framesTotal;
	
	bool				bPlay;
};