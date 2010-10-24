/*
 *  ContourMap.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 24/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "Contour.h"

class ContourMap : public ofxSprite
{

public:
	
	 ContourMap( Contour* contour );
	~ContourMap();
	
	//==================================================
	
	Contour*	contour;
	
	ofImage*	ausStroke;
	ofImage*	ausCover;
	
	bool		bSmoothing;
	bool		bDrawPoints;
	bool		bDrawLines;
	bool		bDrawCurves;
	bool		bDrawSimplified;
	bool		bDrawColor;
	
	//==================================================
	
	void setup			();
	void update			();
	void draw			();
	
	void drawContourPoints		( vector<Blob>& blobs );
	void drawContourLines		( vector<Blob>& blobs, bool useBlobColor = false );
	void drawContourCurveLines	( vector<Blob>& blobs, bool useBlobColor = false );
};