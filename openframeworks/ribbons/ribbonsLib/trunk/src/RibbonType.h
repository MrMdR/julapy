/*
 *  RibbonType.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 30/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVec3f.h"

class RibbonType
{

public :
	
	RibbonType();
	~RibbonType();
	
	void loadTrueTypeFont( string fontName, int size = 50 );
	void drawTypeOnRibbon( string copy, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize );
	
private :
	
	void drawLetter( int letter, float xOffset = 0, float yOffset = 0 );
	void drawLetterVertices();
	void clearLetterVertices();
	
	ofTrueTypeFont font;
	int fontSize;
	
	float *ribbonPositions;
	float *ribbonDirections;
	int ribbonLength;
	
	vector <float*> polyVertices;
	int currentStartVertex;
};