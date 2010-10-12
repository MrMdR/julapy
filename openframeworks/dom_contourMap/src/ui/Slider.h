/*
 *  Slider.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxSprite.h"
#include "ofxMSAInteractiveObject.h"

class Slider : public ofxSprite
{

public:
	
	 Slider()	{};
	~Slider()	{};
	
	////////////////////////////////////////////////////
	//	MEMBERS.
	////////////////////////////////////////////////////
	
	ofRectangle					rect;
	ofxMSAInteractiveObject		intObj;
	float						position;
	float						positionNew;
	float						positionEase;
	
	////////////////////////////////////////////////////
	//	SETUP.
	////////////////////////////////////////////////////
	
	void setup ()
	{
		rect.width	= (int)( ofGetWidth()  * 0.5 );
		rect.height	= (int)( ofGetHeight() * 0.05 );
		rect.x		= (int)( ( ofGetWidth() - rect.width ) * 0.5 );
		rect.y		= (int)( ofGetHeight() - rect.height * 2 );
		
		intObj.setPos( rect.x, rect.y );
		intObj.setSize( rect.width, rect.height );
		intObj.enableMouseEvents();
		
		position		= 0;
		positionNew		= 0;
		positionEase	= 0.2;
	}
	
	////////////////////////////////////////////////////
	//	UPDATE.
	////////////////////////////////////////////////////
	
	void update ()
	{
		if( intObj.isMouseDown() )
		{
			positionNew = ( intObj.getMouseX() - rect.x ) / rect.width;
			positionNew = ofClamp( positionNew, 0, 1 );
		}
		
		position += ( positionNew - position ) * positionEase;
	}
	
	////////////////////////////////////////////////////
	//	DRAW.
	////////////////////////////////////////////////////
	
	void draw ()
	{
		drawBg();
		drawPlayhead();
	}
	
	void drawBg ()
	{
		ofFill();
		ofSetColor( 0xFF0000 );
		ofRect( rect.x, rect.y, rect.width, rect.height );
	}
	
	void drawPlayhead ()
	{
		float w;
		w = 2;
		
		float px;
		px = (int)( rect.x + ( ( rect.width - w ) * position ) );
		
		ofFill();
		ofSetColor( 0x00FF00 );
		ofRect( px, rect.y, w, rect.height );
	}
	
	////////////////////////////////////////////////////
	//	GETTERS.
	////////////////////////////////////////////////////
	
	float getPosition ()
	{
		return position;
	}
};