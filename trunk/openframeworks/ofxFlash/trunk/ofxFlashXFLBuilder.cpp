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
	domType		= DOM_DOCUMENT_TYPE;
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
	
	this->container = container;
	
	if( loadFile( xflFile ) )
	{
		TiXmlElement* child = ( storedHandle.FirstChild() ).ToElement();
		domType = child->Value();
		
		pushTag( domType, 0 );
		
		if( domType == DOM_DOCUMENT_TYPE )
		{
			pushTag( "timelines", 0 );
		}
		else if( domType == DOM_SYMBOL_ITEM_TYPE )
		{
			pushTag( "timeline", 0 );
		}
		
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
		
		return;		// SUPPORT ONLY ONE TIMELINE.
	}
	
	popTag();
}

void ofxFlashXFLBuilder :: buildLayers ()
{
	int numOfLayers;
	numOfLayers = getNumTags( "DOMLayer" );
	
	for( int i=numOfLayers-1; i>=0; i-- )	// work backwards through layers. so when adding to stage, objects sit in right order.
	{
		string layerType;
		layerType = getAttribute( "DOMLayer", "layerType", "", i );
		
		if( layerType == "guide" )		// skip guide layers.
			continue;
		
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
		
		return;		// SUPPORT ONLY THE FIRST FRAME FOR NOW.
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
	
	int numOfElements = 0;
	TiXmlElement* child = ( storedHandle.FirstChildElement() ).ToElement();
	for( numOfElements = 0; child; child = child->NextSiblingElement(), ++numOfElements ) {}
	
	for( int i=numOfElements-1; i>=0; i-- )		// work backwards through elements. so when adding to stage, objects sit in right order.
	{
		TiXmlElement* child = ( storedHandle.ChildElement( i ) ).ToElement();
		string elementTag = child->Value();
		
		if( elementTag == "DOMShape" )
		{
			// NOT SUPPORTED AT THE MOMENT.
		}
		else if( elementTag == "DOMBitmapInstance" )
		{
			string libraryItemName;
			const string* value = new string( child->Attribute( "libraryItemName" ) );
			libraryItemName = *value;
			
			TiXmlHandle isRealHandle = storedHandle.ChildElement( i );		//-- pushTag custom.
			if( isRealHandle.ToNode() )
			{
				storedHandle = isRealHandle;
				level++;
			}
			
			buildBitmap( libraryItemName );
			
			popTag();
		}
		else if( elementTag == "DOMSymbolInstance" )
		{
			string libraryItemName;
			const string* value = new string( child->Attribute( "libraryItemName" ) );
			libraryItemName = *value;
			
			TiXmlHandle isRealHandle = storedHandle.ChildElement( i );		//-- pushTag custom.
			if( isRealHandle.ToNode() )
			{
				storedHandle = isRealHandle;
				level++;
			}
			
			buildMovieClip( libraryItemName );
			
			popTag();
		}
	}
}

void ofxFlashXFLBuilder :: buildBitmap ( string libraryItemName )
{
	ofImage* bitmapImage;
	bitmapImage = (ofImage*)ofxFlashLibrary :: getInstance()->getAsset( libraryItemName );
	
	ofxFlashBitmap* bm;
	bm = new ofxFlashBitmap( bitmapImage );

	setupDisplayObject( bm );
	
	container->addChild( bm );
}

void ofxFlashXFLBuilder :: buildMovieClip ( string libraryItemName )
{
	string libraryItemPath;
	libraryItemPath = xflFolder;
	libraryItemPath += ( domType == DOM_DOCUMENT_TYPE ) ? "LIBRARY/" : "";
	libraryItemPath += libraryItemName;
	libraryItemPath += ".xml";
	
	ofxFlashMovieClip* mc;
	mc = new ofxFlashMovieClip();
	
	setupDisplayObject( mc );
	
	container->addChild( mc );
	
	ofxFlashXFLBuilder* builder;
	builder = new ofxFlashXFLBuilder();
	builder->build( libraryItemPath, mc );
	
	delete builder;
	builder = NULL;
}

void ofxFlashXFLBuilder :: setupDisplayObject ( ofxFlashDisplayObject* displayObject )
{
	if( pushTag( "matrix", 0 ) )
	{
		float tx = getAttribute( "Matrix", "tx", 0.0, 0 );
		float ty = getAttribute( "Matrix", "ty", 0.0, 0 );
		float tz = getAttribute( "Matrix", "tz", 0.0, 0 );
		
		displayObject->x = tx;
		displayObject->y = ty;
		displayObject->z = tz;
		
		displayObject->globalX = container->x + tx;
		displayObject->globalY = container->y + ty;
		displayObject->globalZ = container->z + tz;
		
		popTag();
	}
}