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
		domTimeline.name			= getAttribute( "DOMTimeline", "name",			"", i );
		domTimeline.currentFrame	= getAttribute( "DOMTimeline", "currentFrame",	1,  i );
		
		pushTag( "DOMTimeline", i );
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
	
	//-- count total number of frames in movie clip.
	
	int totalFrames;
	totalFrames = 1;
	
	for( int i=0; i<numOfLayers; i++ )
	{
		string layerType;
		layerType = getAttribute( "DOMLayer", "layerType", "", i );
		
		if( layerType == "guide" )		// skip guide layers.
			continue;
		
		pushTag( "DOMLayer", i );
		pushTag( "frames", 0 );
		
		int numOfFrames;
		numOfFrames = getNumTags( "DOMFrame" );
		
		for( int j=0; j<numOfFrames; j++ )
		{
			int frameIndex		= getAttribute( "DOMFrame", "index",	0, j );
			int frameDuration	= getAttribute( "DOMFrame", "duration",	1, j );
			int frameEnd		= frameIndex + frameDuration;
			
			if( frameEnd > totalFrames )
			{
				totalFrames = frameEnd;
			}
		}
		
		popTag();
		popTag();
	}
	
	if( container->typeID == OFX_FLASH_MOVIE_CLIP_TYPE )		// stage currently only extends ofxFlashDisplayObjectContainer.
	{															// perhaps it should extend ofxFlashMovieClip - will look into this.
		ofxFlashMovieClip* mc;
		mc = (ofxFlashMovieClip*)container;
		mc->setup( totalFrames );
	}
	
	//-- build frames.
	
	for( int i=numOfLayers-1; i>=0; i-- )	// work backwards through layers. so when adding to stage, objects sit in right order.
	{
		domLayer.name			= getAttribute( "DOMLayer", "name",			"",		i );
		domLayer.color			= getAttribute( "DOMLayer", "color",		0,		i );
		domLayer.locked			= getAttribute( "DOMLayer", "locked",		false,  i );
		domLayer.current		= getAttribute( "DOMLayer", "current",		false,  i );
		domLayer.isSelected		= getAttribute( "DOMLayer", "isSelected",	false,  i );
		domLayer.autoNamed		= getAttribute( "DOMLayer", "autoNamed",	false,  i );
		domLayer.layerType		= getAttribute( "DOMLayer", "layerType",	"",		i );
		
		if( domLayer.layerType == "guide" )		// skip guide layers.
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
		domFrame.index				= getAttribute( "DOMFrame", "index",			0,		i );
		domFrame.duration			= getAttribute( "DOMFrame", "duration",			1,		i );
		domFrame.tweenType			= getAttribute( "DOMFrame", "tweenType",		"",		i );
		domFrame.motionTweenSnap	= getAttribute( "DOMFrame", "motionTweenSnap",	false,	i );
		domFrame.keyMode			= getAttribute( "DOMFrame", "keyMode",			0,		i );
		
		pushTag( "DOMFrame", i );
		pushTag( "elements", 0 );
		
		buildElements();
		
		popTag();
		popTag();
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
			domBitmapInstance.libraryItemName	= *new string( child->Attribute( "libraryItemName" ) );
			domBitmapInstance.name				= "";
			domBitmapInstance.referenceID		= "";
			
			TiXmlHandle isRealHandle = storedHandle.ChildElement( i );		//-- pushTag custom.
			if( isRealHandle.ToNode() )
			{
				storedHandle = isRealHandle;
				level++;
			}
			
			buildBitmap();
			
			popTag();
		}
		else if( elementTag == "DOMSymbolInstance" )
		{
			domSymbolInstance.libraryItemName	= *new string( child->Attribute( "libraryItemName" ) );
			domSymbolInstance.name				= "";
			domSymbolInstance.centerPoint3DX	= 0.0;
			domSymbolInstance.centerPoint3DY	= 0.0;
			
			TiXmlHandle isRealHandle = storedHandle.ChildElement( i );		//-- pushTag custom.
			if( isRealHandle.ToNode() )
			{
				storedHandle = isRealHandle;
				level++;
			}
			
			buildMovieClip();
			
			popTag();
		}
	}
}

void ofxFlashXFLBuilder :: buildBitmap ()
{
	ofImage* bitmapImage;
	bitmapImage = (ofImage*)ofxFlashLibrary :: getInstance()->getAsset( domBitmapInstance.libraryItemName );
	
	ofxFlashBitmap* bm;
	bm = new ofxFlashBitmap( bitmapImage );
	bm->libraryItemName = domBitmapInstance.libraryItemName;

	setupDisplayObject( bm );
	
	if( container->typeID == OFX_FLASH_MOVIE_CLIP_TYPE )		// stage currently only extends ofxFlashDisplayObjectContainer.
	{															// perhaps it should extend ofxFlashMovieClip - will look into this.
		int i = domFrame.index;
		int t = domFrame.index + domFrame.duration;
		for( i; i<t; i++ )
		{
			ofxFlashDisplayObjectContainer* frame;
			frame = ( (ofxFlashMovieClip*)container )->getFrameContainer( i );
			frame->addChild( bm );
		}
	}
	else if( container->typeID == OFX_FLASH_STAGE_TYPE )
	{
		container->addChild( bm );
	}
}

void ofxFlashXFLBuilder :: buildMovieClip ()
{
	string libraryItemPath;
	libraryItemPath = xflFolder;
	libraryItemPath += ( domType == DOM_DOCUMENT_TYPE ) ? "LIBRARY/" : "";
	libraryItemPath += domSymbolInstance.libraryItemName;
	libraryItemPath += ".xml";

	ofxFlashMovieClip* mc;
	mc = new ofxFlashMovieClip();
	mc->libraryItemName = domSymbolInstance.libraryItemName;
	
	setupDisplayObject( mc );
	
	if( container->typeID == OFX_FLASH_MOVIE_CLIP_TYPE )		// stage currently only extends ofxFlashDisplayObjectContainer.
	{															// perhaps it should extend ofxFlashMovieClip - will look into this.
		int i = domFrame.index;
		int t = domFrame.index + domFrame.duration;
		for( i; i<t; i++ )
		{
			ofxFlashDisplayObjectContainer* frame;
			frame = ( (ofxFlashMovieClip*)container )->getFrameContainer( i );
			frame->addChild( mc );
		}
	}
	else if( container->typeID == OFX_FLASH_STAGE_TYPE )
	{
		container->addChild( mc );
	}
	
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