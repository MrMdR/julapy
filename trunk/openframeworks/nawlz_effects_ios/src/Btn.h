/*
 *  Btn.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 26/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxFlash.h"

class Btn
{
	
public:
	
	Btn( ofxFlashMovieClip* asset, int appID, int index )
	{
		this->asset = asset;
		asset->mouseEnabled( true );
        
        this->appID = appID;
        this->index = index;
		
		bPressed		= false;
		bPressedChanged	= false;
        bSelected       = false;
	}
	
	~Btn()
	{
		//
	}
	
	ofxFlashMovieClip* asset;
	bool bPressed;
	bool bPressedChanged;
    bool bSelected;
    int appID;
    int index;
	
	void update ()
	{
		bPressedChanged	= bPressed != asset->mouseDown();
		bPressed		= asset->mouseDown();
	}
	
	bool isPressed ()
	{
		return bPressed && bPressedChanged;
	}
	
    void draw ()
    {
        string btnNo;
        btnNo = ofToString( index, 0 );
        
        if( bSelected )
            ofSetColor( 255, 0, 0 );
        else
            ofSetColor( 255, 255, 255 );
        
        int x = asset->x() + 9;
        int y = asset->y() + 17;
        
        if( btnNo.size() > 1 )
            x -= 4;
        
        ofDrawBitmapString( btnNo, x, y );
    }
	
};