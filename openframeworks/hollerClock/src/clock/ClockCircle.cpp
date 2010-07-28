/*
 *  ClockCircle.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockCircle.h"

ClockCircle :: ClockCircle ( float r, int c )
{
	radius		= r;
	colorHex	= c;
	
	colorTrgt.r	= colorCurr.r = ( c >> 16 ) & 0xff;
	colorTrgt.g	= colorCurr.g = ( c >> 8  ) & 0xff;
	colorTrgt.b	= colorCurr.b = ( c >> 0  ) & 0xff;
	
	spinDir = ( ofRandom( 0.0, 1.0 ) > 0.5 ) ? 1 : -1;
	spinFrc	= ofRandom( 0.2, 0.5 );
	
	active		= false;
	
	joint = NULL;
	point = NULL;
}

ClockCircle :: ~ClockCircle ()
{
	//
}

///////////////////////////////////////////////
//	SIZE.
///////////////////////////////////////////////

void ClockCircle :: setSize ( ofRectangle &size )
{
	setSize( size.width, size.height );
}

void ClockCircle :: setSize ( int w, int h )
{
	screenWidth		= w;
	screenHeight	= h;
}

///////////////////////////////////////////////
//	JOINT.
///////////////////////////////////////////////

void ClockCircle :: createJoint ()
{
	if( hasJoint() )
		return;
	
	b2BodyDef bd;
	bd.position.Set( ( lineUpPoint.x * screenWidth ) / OFX_BOX2D_SCALE, ( lineUpPoint.y * screenHeight ) / OFX_BOX2D_SCALE );
	point = world->CreateBody( &bd );
	
	b2DistanceJointDef jd;
	b2Vec2 p1, p2, d;
	
	jd.frequencyHz  = 1.0;
	jd.dampingRatio = 0.2;
	
	jd.body1 = body;
	jd.body2 = point;
	
	jd.localAnchor1.Set( 0, 0 );
	jd.localAnchor2.Set( 0, 0 );
	jd.collideConnected = false;
	
	p1	= jd.body1->GetWorldPoint( jd.localAnchor1 );
	p2	= jd.body2->GetWorldPoint( jd.localAnchor2 );
	d	= p2 - p1;
	
	jd.length = 0;
	
	joint = (b2DistanceJoint*)world->CreateJoint( &jd );
	
	body->WakeUp();
}

void ClockCircle :: destroyJoint ()
{
	if( !hasJoint() )
		return;
	
	world->DestroyJoint( joint );
	joint = NULL;
	
	world->DestroyBody( point );
	point = NULL;
}

bool ClockCircle :: hasJoint ()
{
	return ( point != NULL && joint != NULL );
}

///////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////

void ClockCircle :: update ()
{
	float angle			= getRotation();
	const b2XForm& xf	= body->GetXForm();
	b2Vec2	center		= body->GetPosition();
	b2Vec2	axis		= xf.R.col1;
	b2Vec2	p			= center + ( radius * 0.6 ) / OFX_BOX2D_SCALE * axis;
	
	eye.x = p.x * OFX_BOX2D_SCALE;
	eye.y = p.y * OFX_BOX2D_SCALE;
	
	colorCurr.r += ( colorTrgt.r - colorCurr.r ) * 0.1;
	colorCurr.g += ( colorTrgt.g - colorCurr.g ) * 0.1;
	colorCurr.b += ( colorTrgt.b - colorCurr.b ) * 0.1;
}

///////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////

void ClockCircle :: draw ()
{
	if( false )
	{
		ofxBox2dCircle :: draw();
		
		return;
	}
	
	float radius = getRadius();
	
	glPushMatrix();
	glTranslatef( getPosition().x, getPosition().y, 0 );
	
	ofCircle( 0, 0, radius - 1 );
	
	glPopMatrix();

	//--
	
	glPushMatrix();
	glTranslatef( eye.x, eye.y, 0 );
	
	int col = 220;
	
	ofSetColor( col, col, col );
	ofCircle( 0, 0, radius * 0.1 );
	
	
	glPopMatrix();
}