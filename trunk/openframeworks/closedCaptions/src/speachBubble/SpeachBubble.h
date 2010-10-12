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

#define SPEACH_BUBBLE_RESOLUTION  100

class SpeachBubble
{

public :
	
	 SpeachBubble();
	~SpeachBubble();
	
	void init			( string str, ofTrueTypeFont* font );
	void setPosition	( ofPoint& p );
	void setTarget		( ofPoint& p );
	void setVolume		( float v );
	void setVolumeData	( float *v );
	void playIn			();
	void playOut		();
	void update			();
	
	ofRectangle getRect	();
	
	void drawBubble		();
	void drawPoint		();
	void drawText		();
	
	string			bubbleStr;
	ofPoint			position;
	ofPoint			target;
	ofRectangle		rect;
	ofRectangle		rectText;
	float			scale;
	float			volume;
	float			volumePush;
	float			*volumeData;
	int				volumeDir;
	ofTrueTypeFont	*font;
	
	bool			bPlayingIn;
	bool			bPlayingOut;
	bool			bFinished;
};

#endif;