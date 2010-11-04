/*
 *  ofxFlashXFLBuilder.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 5/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFlashXFLBuilder.h"

ofxFlashXFLBuilder :: ofxFlashXFLBuilder()
{
	xflFile		= "";
	xflFolder	= "";
	container	= NULL;
}

ofxFlashXFLBuilder :: ~ofxFlashXFLBuilder()
{
	//
}

///////////////////////////////////////////
//
///////////////////////////////////////////



///////////////////////////////////////////
//	BUILD.
///////////////////////////////////////////

void ofxFlashXFLBuilder :: build ( const string& file, ofxFlashDisplayObjectContainer* container )
{
	vector<string> xflFileSplit;
	xflFile			= file;
	xflFileSplit	= ofSplitString( xflFile, "/" );
	xflFolder		= "";
	for( int i=0; i<xflFileSplit.size()-1; i++ )	// drop the file
	{
		xflFolder += xflFileSplit[ i ] + "/";
	}
	
	if( loadFile( xflFile ) )
	{
		pushTag( "DOMDocument", 0 );
		pushTag( "timelines", 0 );
		
		buildTimelines();
		
		popTag();
		popTag();
	}
}

void ofxFlashXFLBuilder :: buildTimelines ()
{
	int numOfTimelines;
	numOfTimelines = getNumTags( "DOMTimeline" );
	
	for( int i=0; i<numOfTimelines; i++ )
	{
		pushTag( "DOMTimeline", 0 );
		pushTag( "layers", 0 );
		
		buildLayers();
		
		popTag();
		popTag();
		
		return;		// only one timeline supported for now.
	}
	
	popTag();
}

void ofxFlashXFLBuilder :: buildLayers ()
{
	int numOfLayers;
	numOfLayers = getNumTags( "DOMLayer" );
	
	for( int i=numOfLayers-1; i>=0; i-- )	// work backwards through layers. so when adding to stage, objects sit in right order.
	{
		pushTag( "DOMLayer", i );
		pushTag( "frames", 0 );
		
		buildFrames();
		
		popTag();
		popTag();
	}
}

void ofxFlashXFLBuilder :: buildFrames ()
{
	int numOfFrames;
	numOfFrames = getNumTags( "DOMFrame" );
	
	for( int i=0; i<numOfFrames; i++ )
	{
		pushTag( "DOMFrame", 0 );
		pushTag( "elements", 0 );
		
		buildElements();
		
		popTag();
		popTag();
		
		return;		// only one frame supported for now.
	}
}

void ofxFlashXFLBuilder :: buildElements ()
{
	// there a few different elements types.
	//
	// - DOMShape
	// - DOMBitmapInstance
	// - DOMSymbolInstance (MovieClip)
	//
	// we are only interested in MovieClips for now.
	
	int numOfElements;
	numOfElements = getNumTags( "DOMSymbolInstance" );
	
	for( int i=numOfElements-1; i>=0; i-- )		// work backwards through elements. so when adding to stage, objects sit in right order.
	{
		pushTag( "DOMSymbolInstance", i );
		
		ofxFlashMovieClip* mc;
		mc = new ofxFlashMovieClip();
		container->addChild( mc );
		
		string libraryItemName;
		libraryItemName = getAttribute( "DOMSymbolInstance", "libraryItemName",	"", i );
		
		string libraryItemPath;
		libraryItemPath = libraryItemName + ".xml";
		
		ofxFlashXFLBuilder* builder;
		builder = new ofxFlashXFLBuilder();
		builder->build( libraryItemPath, mc );
		
		delete builder;
		builder = NULL;
		
		popTag();
	}
}