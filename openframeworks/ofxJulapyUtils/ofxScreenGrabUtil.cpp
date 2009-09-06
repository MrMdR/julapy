/*
 *  ofxScreenGrabUtil.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxScreenGrabUtil.h"

ofxScreenGrabUtil :: ofxScreenGrabUtil()
{
	fileName		= "image";
	recording		= false;
	counter			= 0;
	
	saveArea.x		= 0;
	saveArea.y		= 0;
	saveArea.width	= ofGetWidth();
	saveArea.height	= ofGetHeight();
}

ofxScreenGrabUtil :: ~ofxScreenGrabUtil()
{
	
}

void ofxScreenGrabUtil :: setup( string name, ofRectangle *rect )
{
	fileName = name;
	
	saveArea.x		= rect->x;
	saveArea.y		= rect->y;
	saveArea.width	= rect->width;
	saveArea.height	= rect->height;
}

void ofxScreenGrabUtil :: start( )
{
	recording	= true;
	counter		= 0;
}

void ofxScreenGrabUtil :: save()
{
	image.grabScreen( saveArea.x, saveArea.y, saveArea.width, saveArea.height );
	
	string imageFileNameStr = fileName;
	char imageFileName[ 255 ];
	sprintf( imageFileName, "_%02d%02d%02d_%05d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), counter );
	imageFileNameStr.insert( imageFileNameStr.size(), imageFileName );

	image.saveImage( imageFileNameStr );
	
	counter++;
}

void ofxScreenGrabUtil :: stop()
{
	recording = false;
}

bool ofxScreenGrabUtil :: isRecording()
{
	return recording;
}

void ofxScreenGrabUtil :: setArea( ofRectangle *rect )
{
	saveArea.x		= rect->x;
	saveArea.y		= rect->y;
	saveArea.width	= rect->width;
	saveArea.height	= rect->height;
}