/*
 *  ofxContourUtil.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 17/05/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OFX_CONTOUR_UTIL
#define OFX_CONTOUR_UTIL

#include "ofMain.h"
#include "ofxVectorMath.h"			// TODO :: need to get rid of this dependency - its just simplify() that uses ofxVectorMath.
#include "polylineSimplify.h"
#include "convexHull.h"

class ofxContourUtil
{

public :
	
	void smooth		( vector<ofPoint>& contourIn, float smoothPct );
	void smooth		( vector<ofPoint>& contourIn, vector<ofPoint>& contourOut, float smoothPct );
	
	void simplify	( vector<ofPoint>& contourIn, float tolerance );
	void simplify	( vector<ofPoint>& contourIn, vector<ofPoint>& contourOut, float tolerance );
	
	void convexHull	( vector<ofPoint>& contourIn );
	void convexHull	( vector<ofPoint>& contourIn, vector<ofPoint>& contourOut );
	
};

#endif