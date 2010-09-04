/*
 *  ColorCircle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorCircle.h"

ColorCircle :: ColorCircle ()
{
	ofxVec2f dir;
	dir.x = ofRandom( -1, 1 );
	dir.y = ofRandom( -1, 1 );
	dir.normalize();
	
	float minLength = 0.1;
	float rndLength	= 0.2;
	
	ofxVec2f vec;
	vec = dir;
	vec *= minLength;
	
	dir *= ofRandom( 0.0, rndLength );
	vec += dir;
	
//	posOffset.x = vec.x;
//	posOffset.y = vec.y;
	
	posOffset.x = 0;
	posOffset.y = 0;
	
	collision = 0;
	
	bSelected = false;
}

void ColorCircle :: setScreen ( ofxScreen screen )
{
	this->screen = screen;
}

void ColorCircle :: setColor ( const ofColor& color )
{
	this->color.r = color.r;
	this->color.g = color.g;
	this->color.b = color.b;
}

void ColorCircle :: init ()
{

}

void ColorCircle :: update ()
{
	pos			= getPosition();
	posNorm.x	= pos.x / (float)screen.screenWidth;
	posNorm.y	= pos.y / (float)screen.screenHeight;
	
	float px = posNorm.x + posOffset.x;
	float py = posNorm.y + posOffset.y;
	
	px = MIN( MAX( px, 0.0 ), 1.0 );
	py = MIN( MAX( py, 0.0 ), 1.0 );
	
	posColor.x = px;
	posColor.y = py;
	
	collision *= 0.96;
}

void ColorCircle :: draw ()
{
	if( false )
	{
		ofxBox2dCircle :: draw ();
		return;
	}
	
//	drawFill();
	drawStroke();
//	drawColorLine();
}

void ColorCircle :: drawFill ()
{
	ofFill();
	ofSetColor( color.r, color.g, color.b );
	ofCircle( pos.x, pos.y, getRadius() );
}

void ColorCircle :: drawStroke ()
{
	bool bSmooth;
	bSmooth = true;
#ifdef TARGET_OF_IPHONE
	bSmooth = false;
#endif
	
	if( bSmooth )
		ofEnableSmoothing();
	
	ofNoFill();
//	ofSetColor( color.r, color.g, color.b );
	ofSetColor( 255, 255, 255 );
	ofCircle( pos.x, pos.y, getRadius() );

	if( bSmooth )
		ofDisableSmoothing();
}

void ColorCircle :: drawColorLine ()
{
	bool bSmooth;
	bSmooth = true;
#ifdef TARGET_OF_IPHONE
	bSmooth = false;
#endif
	
	if( bSmooth )
		ofEnableSmoothing();
	
	float cx = posColor.x * screen.screenWidth;
	float cy = posColor.y * screen.screenHeight;
	
	ofNoFill();
	ofSetColor( 255, 255, 255 );
	ofLine( pos.x, pos.y, cx, cy );
	
	if( bSmooth )
		ofDisableSmoothing();
	
}