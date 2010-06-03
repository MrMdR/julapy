/*
 *  SpeachBubble.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 2/06/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SPEACH_BUBBLE
#define SPEACH_BUBBLE

#include "ofMain.h"

class SpeachBubble
{

public :
	
	 SpeachBubble();
	~SpeachBubble();
	
	void init			( string str, ofTrueTypeFont* font );
	void setPosition	( ofPoint& p );
	void setSize		( ofRectangle& r );
	void playIn			();
	void playOut		();
	void update			();
	
	void drawBubble		();
	void drawText		();
	
	string			bubbleStr;
	ofPoint			pos;
	ofRectangle		rect;
	ofRectangle		rectPad;
	float			scale;
	ofTrueTypeFont	*font;
	
	bool			bPlayingIn;
	bool			bPlayingOut;
	bool			bFinished;
};

#endif;