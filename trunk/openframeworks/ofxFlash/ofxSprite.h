/*
 *  ofxSprite.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OFX_FLASH_SPRITE
#define OFX_FLASH_SPRITE

#include "ofxFlash.h"

class ofxSprite
{	
	
public :
	
	 ofxSprite();
	~ofxSprite();
	
	virtual void setup	();
	virtual void update	();
	virtual void draw	();
	virtual void clear	();
	
	ofRectangle	size;
	
};

#endif