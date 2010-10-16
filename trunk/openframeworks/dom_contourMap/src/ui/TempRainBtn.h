/*
 *  TempRainBtn.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxSprite.h"
#include "ofxMSAInteractiveObject.h"

class TempRainBtn : public ofxSprite
{
	
public:
	
	 TempRainBtn()	{};
	~TempRainBtn()	{};
	
	////////////////////////////////////////////////////
	//	MEMBERS.
	////////////////////////////////////////////////////
	
	ofRectangle					rect;
	ofxMSAInteractiveObject		intObj;
	ofImage						imageTempState;
	ofImage						imageRainState;
	
	bool						bMouseDown;
	bool						bTemp;
	
	////////////////////////////////////////////////////
	//	SETUP.
	////////////////////////////////////////////////////
	
	void setup ()
	{
		rect.width	= (int)( 50 );
		rect.height	= (int)( 50 );
		rect.x		= (int)( 180 );
		rect.y		= (int)( ofGetHeight() - 80 );
		
		intObj.setPos( rect.x, rect.y );
		intObj.setSize( rect.width, rect.height );
		intObj.enableMouseEvents();
		
		imageTempState.loadImage( "ui/TempRainBtn_T.png" );
		imageRainState.loadImage( "ui/TempRainBtn_R.png" );
		
		bMouseDown	= false;
		bTemp		= false;
	}
	
	////////////////////////////////////////////////////
	//	UPDATE.
	////////////////////////////////////////////////////
	
	void update ()
	{
		if( bMouseDown != intObj.isMouseDown() )		// change has occured.
		{
			bMouseDown = intObj.isMouseDown();
			
			if( bMouseDown )
			{
				bTemp = !bTemp;
			}
		}
	}
	
	bool isTemp ()
	{
		return bTemp;
	}
	
	////////////////////////////////////////////////////
	//	DRAW.
	////////////////////////////////////////////////////
	
	void draw ()
	{
		ofFill();
		ofSetColor( 0xFF0000 );
		ofRect( rect.x, rect.y, rect.width, rect.height );
		
		ofSetColor( 0xFFFFFF );
		if( bTemp )
		{
			imageTempState.draw( rect.x, rect.y );
		}
		else 
		{
			imageRainState.draw( rect.x, rect.y );
		}
		
	}
	
};