/*
 *  ofxStage.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxSprite.h"

class ofxStage : public ofxSprite
{	
	
public:
	
	 ofxStage();
	~ofxStage();
	
	void addListeners		();
	void removeListeners	();
	
	virtual void update		();
	virtual void draw		();
	
	///////////////////////////////////////////////
	//
	//	PRIVATE.
	//
	///////////////////////////////////////////////
	
private:
	
	void update	( ofEventArgs &e );
	void draw	( ofEventArgs &e );

	void updateChildren	( vector<ofxSprite*>& children );
	void drawChildren	( vector<ofxSprite*>& children );
};