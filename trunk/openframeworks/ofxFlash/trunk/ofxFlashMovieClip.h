/*
 *  ofxFlashMovieClip.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxFlash.h"

class ofxFlashMovieClip : public ofxFlashSprite
{

public:
	
	 ofxFlashMovieClip();
	~ofxFlashMovieClip();
	
	virtual void setup			( int totalFrames = 1 );
	virtual void update			();
	virtual void draw			();
	
	virtual void gotoAndPlay	( int frameNum );
	virtual void gotoAndStop	( int frameNum );
	virtual void nextFrame		();
	virtual void prevFrame		();
	virtual void play			();
	virtual void stop			();
	
	int totalFrames		();
	int currentFrame	();
	
	ofxFlashDisplayObjectContainer* getFrameContainer( int frameIndex );
	
private:
	
	void addFrameChildren		();
	void removeFrameChildren	();
	
	vector<ofxFlashDisplayObjectContainer*>	frames;
	ofxFlashDisplayObjectContainer*			frame;
	
	int		frameIndex;
	bool	bPlay;
	
};