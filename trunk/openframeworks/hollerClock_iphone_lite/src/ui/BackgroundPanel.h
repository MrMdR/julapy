/*
 *  BackgroundPanel.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 11/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxFlashMovieClip.h"

class BackgroundPanel
{

public:
	
	 BackgroundPanel( ofxFlashMovieClip* asset );
	~BackgroundPanel();
	
	void checkTileIsOnScreen( ofRectangle& screenRect );
	
	ofxFlashMovieClip* asset;
	int ix;
	int iy;
	
	ofRectangle	rect;
	bool bOnScreen;
};