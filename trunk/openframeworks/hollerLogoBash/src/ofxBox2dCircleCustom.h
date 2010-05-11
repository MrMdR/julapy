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
	
	ofImage* circleLayer1;
	ofImage* circleLayer2;
	ofImage* circleLayer3;
	ofImage* circleLayer4;
	
	virtual void setImages( ofImage* layer1, ofImage* layer2, ofImage* layer3, ofImage* layer4 )
	{
		circleLayer1 = layer1;
		circleLayer2 = layer2;
		circleLayer3 = layer3;
		circleLayer4 = layer4;
	}
	
	virtual void drawImage ()
	{
		float scl;
		scl = getRadius() / (float)circleLayer1->width;
		
		glPushMatrix();
		glTranslatef
		(
			getPosition().x,
			getPosition().y,
			0
		);
		
		float r;
		r = getRotation();
		
		if( r >= 0 )
			r += 180;
		
		glRotatef( r, 0, 0, 1 );
		glScalef( scl * 2, scl * 2, 0 );
		
		ofEnableSmoothing();
		ofEnableAlphaBlending();

		ofSetColor( color );
		
		float cx, cy;
		cx = -circleLayer1->width  * 0.5;
		cy = -circleLayer1->height * 0.5;
		
		circleLayer1->draw( cx, cy );
		
		ofSetColor( 0xFFFFFF );
		
		circleLayer2->draw( cx, cy );
		circleLayer3->draw( cx, cy );
		circleLayer4->draw( cx, cy );

		ofDisableSmoothing();
		ofDisableAlphaBlending();
		
		glPopMatrix();
	}
	
	virtual void draw()
	{
		if( dead && !body )
			return;
		
		float radius;
		radius = getRadius();
		radius *= 0.9;			// smaller for extra space between objects.
		
		glPushMatrix();
		glTranslatef( getPosition().x, getPosition().y, 0 );
		
		ofSetColor( color );
		ofCircle( 0, 0, radius );
		
		glPopMatrix();
	}
	
	
	
};