/*
 *  FaceItem.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  http://julapy.com/blog/
 *
 */

#pragma once

#include "ofMain.h"

class ofxCvHaarTrackerItem
{
	
public :
	
	ofxCvHaarTrackerItem();
	~ofxCvHaarTrackerItem();
	
	void setID( int hid );
	int  getID();
	
	void set			( const ofRectangle& haarRect );
	void add			( const ofRectangle& haarRect );
	
	bool checkItem		( const ofRectangle& haarRect );
	bool hasBeenMatched	();
	bool isItemIdle		();
	bool isValid		();
	
	void update();
	void easeItemSize( float easeScale );
	void easeItemPosition( float easeScale );
	
	int				itemID;
	
	int				rectBufferCount;
	int				rectBufferMin;
	int				rectBufferLimit;
	
	vector<ofRectangle>		rectBuffer;
	ofRectangle				rect;
	ofRectangle				rectEase;
	
	Boolean			isMatch;
	Boolean			isIdle;
	int				idleCount;
	int				idleCountLimit;
	int				centreDistanceLimit;
	
private :
	
	bool checkAgainstCentreDistance( float cx, float cy );
	void clearRectBuffer();

};