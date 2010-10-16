/*
 *  PlayBtn.h
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

class PlayBtn : public ofxSprite
{

public:
	
	 PlayBtn()	{};
	~PlayBtn()	{};
	
	////////////////////////////////////////////////////
	//	MEMBERS.
	////////////////////////////////////////////////////
	
	ofRectangle					rect;
	ofxMSAInteractiveObject		intObj;
	ofImage						imagePlayState;
	ofImage						imagePauseState;
	bool						bMouseDown;
	bool						bPlay;

	////////////////////////////////////////////////////
	//	SETUP.
	////////////////////////////////////////////////////
	
	void setup ()
	{
		rect.width	= (int)( 50 );
		rect.height	= (int)( 50 );
		rect.x		= (int)( 250 );
		rect.y		= (int)( ofGetHeight() - 80 );
		
		intObj.setPos( rect.x, rect.y );
		intObj.setSize( rect.width, rect.height );
		intObj.enableMouseEvents();
		
		imagePlayState.loadImage( "ui/PlayBtn_PlayState.png" );
		imagePauseState.loadImage( "ui/PlayBtn_PauseState.png" );
		
		bMouseDown	= false;
		bPlay		= false;
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
				bPlay = !bPlay;
			}
		}
	}
	
	bool isPlaying ()
	{
		return bPlay;
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
		if( bPlay )
		{
			imagePlayState.draw( rect.x, rect.y );
		}
		else 
		{
			imagePauseState.draw( rect.x, rect.y );
		}

	}
	
};