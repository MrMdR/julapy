/*
 *  FaceTracker.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  http://julapy.com/blog/
 *
 */

#ifndef OFX_CV_HAAR_TRACKER
#define OFX_CV_HAAR_TRACKER

#include "ofMain.h"
#include "ofxCvMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxCvHaarTrackerItem.h"

class ofxCvHaarTracker
{

public :	

	ofxCvHaarTracker();
	~ofxCvHaarTracker();
	
	ofxCvHaarFinder* setup		( string haarFile );
	ofxCvHaarFinder* setup		( ofxCvHaarFinder* hf );
	
	void findHaarObjects		( ofxCvGrayscaleImage &image );
	bool hasNextHaarItem		();
	int	 getHaarItemID			();
	void getHaarItemRect		( ofRectangle& haarRect );
	void getHaarItemRectEased	( ofRectangle& haarRect );
	void clearHaarItems			();
	
	ofxCvHaarFinder	*haarFinder;
	
protected :	
	
	void init ();
	
	vector<ofxCvHaarTrackerItem>		haarItems;
	int									haarItemIndex;
	int									haarItemIteratorIndex;
	
	int									haarItemIDCounter;
	int									haarItemIDCounterLimit;
	
};

#endif