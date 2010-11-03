/*
 *  ofxFlashFXL.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 3/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"
#include "ofxXmlSettings.h"

class ofxFlashXFL
{

public:
	
	 ofxFlashXFL();
	~ofxFlashXFL();
	
	ofxXmlSettings	xml;
	string			pathToXFL;
	
	void loadXFL			( string pathToXFL );
	void loadAssets			();
	void build				();
	
private:
	
	void loadXFLMedia		();
	
	int  determineMediaType	( string fileName );
	
};