/*
 *  ofxFlashXFLBuilder.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 5/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"
#include "ofxXmlSettings.h"

#define DOM_DOCUMENT_TYPE		"DOMDocument"
#define DOM_SYMBOL_ITEM_TYPE	"DOMSymbolItem"

class ofxFlashXFLBuilder : public ofxXmlSettings
{

public:
	
	 ofxFlashXFLBuilder();
	~ofxFlashXFLBuilder();
	
	void build ( const string& xflFile, ofxFlashDisplayObjectContainer* container );
	
private:
	
	string	xflFile;
	string	xflFolder;
	ofxFlashDisplayObjectContainer* container;
	string	domType;
	
	void buildTimelines		();
	void buildLayers		();
	void buildFrames		();
	void buildElements		();
	void buildBitmap		( string libraryItemName );
	void buildMovieClip		( string libraryItemName );
	void setupDisplayObject ( ofxFlashDisplayObject* displayObject );
	
};