/*
 *  ClockLabel.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 10/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockLabel.h"

ClockLabel :: ClockLabel ()
{
	screenWidth		= ofGetWidth();
	screenHeight	= ofGetHeight();
	forceScale		= 1.0;
	tex				= NULL;
}

ClockLabel :: ~ClockLabel ()
{
	//
}

void ClockLabel :: setSize ( ofRectangle &size )
{
	setSize( size.width, size.height );
}

void ClockLabel :: setSize ( int w, int h )
{
	screenWidth		= w;
	screenHeight	= h;
}

void ClockLabel :: setTexture ( ofTexture* tex )
{
	this->tex = tex;
}

void ClockLabel :: setForceScale ( float scale )
{
	forceScale = scale;
}

void ClockLabel :: init ()
{
	//
}

void ClockLabel :: setup ( b2World* b2dworld, float x, float y, float w, float h, bool isFixed )
{
	if( b2dworld == NULL ) 
	{
		ofLog(OF_LOG_NOTICE, "- must have a valid world -");
		return;
	}
	
	world				= b2dworld;
	bIsFixed			= isFixed;
	
	//Rect Shape
	w/=2; h/=2;
	shape.SetAsBox(w/OFX_BOX2D_SCALE, h/OFX_BOX2D_SCALE);
	
	//Build Body for shape
	b2BodyDef bodyDef;
	
	//set properties
	if(isFixed) {
		shape.density	  = 0;
		shape.restitution = 0;
		shape.friction	  = 0;
	}
	else {
		shape.density	  = mass;
		shape.restitution = bounce;
		shape.friction	  = friction;
	}
	
	// need to check for rect mode
	x += w; y += h;
	bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);	
	
	body = world->CreateBody(&bodyDef);
	body->SetLinearVelocity(b2Vec2(0.0, 0.0));
	body->CreateShape(&shape);
	body->SetMassFromShapes();
	
	createJoint1( body, x, y, w, h );
	createJoint2( body, x, y, w, h );
}

void ClockLabel :: createJoint1 ( b2Body* body, float x, float y, float w, float h )
{
	float dh = -( h * 0.5 ) / OFX_BOX2D_SCALE;
	
	b2BodyDef bd;
	bd.position.Set( screenWidth / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE + dh );
	point1 = world->CreateBody( &bd );
	
	b2DistanceJointDef jd;
	b2Vec2 p1, p2, d;
	
	jd.frequencyHz  = 1.5;
	jd.dampingRatio = 0.1;
	
	jd.body1 = body;
	jd.body2 = point1;
	
	jd.localAnchor1.Set( w / OFX_BOX2D_SCALE, dh );
	jd.localAnchor2.Set( 0, 0 );
	jd.collideConnected = false;
	
	p1	= jd.body1->GetWorldPoint( jd.localAnchor1 );
	p2	= jd.body2->GetWorldPoint( jd.localAnchor2 );
	d	= p2 - p1;
	
	jd.length = ( screenWidth * 0.01 ) / OFX_BOX2D_SCALE;
	
	joint1 = (b2DistanceJoint*)world->CreateJoint( &jd );
	
	body->WakeUp();
}

void ClockLabel :: createJoint2 ( b2Body* body, float x, float y, float w, float h )
{
	float dh = ( h * 0.5 ) / OFX_BOX2D_SCALE;
	
	b2BodyDef bd;
	bd.position.Set( screenWidth / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE + dh );
	point2 = world->CreateBody( &bd );
	
	b2DistanceJointDef jd;
	b2Vec2 p1, p2, d;
	
	jd.frequencyHz  = 1.5;
	jd.dampingRatio = 0.1;
	
	jd.body1 = body;
	jd.body2 = point2;
	
	jd.localAnchor1.Set( w / OFX_BOX2D_SCALE, dh );
	jd.localAnchor2.Set( 0, 0 );
	jd.collideConnected = false;
	
	p1	= jd.body1->GetWorldPoint( jd.localAnchor1 );
	p2	= jd.body2->GetWorldPoint( jd.localAnchor2 );
	d	= p2 - p1;
	
	jd.length = ( screenWidth * 0.01 ) / OFX_BOX2D_SCALE;
	
	joint2 = (b2DistanceJoint*)world->CreateJoint( &jd );
	
	body->WakeUp();
}

void ClockLabel :: draw ()
{
	ofxBox2dRect :: draw();
	
	ofEnableAlphaBlending();
	ofSetColor(255, 0, 255, 200 );
	
	b2Vec2 p1;
	b2Vec2 p2;
	
	p1 = joint1->GetAnchor1();
	p2 = joint1->GetAnchor2();
	p1 *= OFX_BOX2D_SCALE;
	p2 *= OFX_BOX2D_SCALE;
	ofLine(p1.x, p1.y, p2.x, p2.y);	   
	
	p1 = joint2->GetAnchor1();
	p2 = joint2->GetAnchor2();
	p1 *= OFX_BOX2D_SCALE;
	p2 *= OFX_BOX2D_SCALE;
	ofLine(p1.x, p1.y, p2.x, p2.y);	   
	
	ofDisableAlphaBlending();
}
