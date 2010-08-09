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

	static ofxStage* stage();

	virtual void setup	();
	virtual void clear	();
	
	virtual void update	();
	virtual void draw	();
	
private:

	///////////////////////////////////////////
	//	SINGLETON.
	///////////////////////////////////////////
	
	static ofxStage* _instance;

	ofxStage();
	~ofxStage();
	ofxStage(const ofxStage &);					// intentionally undefined
	ofxStage & operator=(const ofxStage &);		// intentionally undefined

	
	///////////////////////////////////////////
	//	
	///////////////////////////////////////////
	
	void updateHandler		( ofEventArgs &e );
	void updateChildren		( vector<ofxSprite*>& children );
	
	void drawHandler		( ofEventArgs &e );
	void drawChildren		( vector<ofxSprite*>& children );
	
};