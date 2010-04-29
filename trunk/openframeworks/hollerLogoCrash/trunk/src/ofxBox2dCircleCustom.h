/*
 *  ofxBox2dCircleCustom.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 29/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBox2dCircle.h"

class ofxBox2dCircleCustom : public ofxBox2dCircle
{

public :
	
	int color;
	
	virtual void draw() {
		if(dead && !body) return;
		
		float radius;
		radius = getRadius();
		radius *= 0.9;			// smaller for extra space between objects.
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofEnableSmoothing();
		
		ofFill();
		ofSetColor( color );
		
		ofCircle( 0, 0, radius );
		
		ofNoFill();
		ofSetColor( color );

		ofCircle( 0, 0, radius );
		
		ofDisableSmoothing();
		
		glPopMatrix();
	}
	
};