/*
 *  Reference.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 21/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

class Reference : public ofxSprite
{

public:
	
	Reference()
	{
		setup();
	}
	
	~Reference() {};
	
	//==================================================
	
	ofImage* ref0;
	ofImage* ref1;
	ofImage* ref2;
	
	//==================================================
	
	void setup ()
	{
		ref0 = (ofImage*)ofxAssets :: getInstance()->getAsset( "ref_01" );
		ref1 = (ofImage*)ofxAssets :: getInstance()->getAsset( "ref_02" );
		ref2 = (ofImage*)ofxAssets :: getInstance()->getAsset( "ref_03" );
	}
	
	void update ()
	{
		
	}
	
	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		ref0->draw( 0, 0 );
	}
};