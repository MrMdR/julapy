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

void ClockCircle :: init ()
{
	float circlePad1;
	float circlePad2;
	circlePad1 = screenHeight * 0.035;
	circlePad2 = screenHeight * 0.060;
	
	pointsTotal = POINT_RES;
	for( int i=0; i<pointsTotal; i++ )
	{
		float p = i / (float)pointsTotal;
		
		points1[ i ].x	= ( radius + circlePad1 ) * sin( p * TWO_PI );
		points1[ i ].y	= ( radius + circlePad1 ) * cos( p * TWO_PI );
		
		points2[ i ].x	= ( radius + circlePad2 ) * sin( p * TWO_PI );
		points2[ i ].y	= ( radius + circlePad2 ) * cos( p * TWO_PI );
	}
	
	//--
	
	for( int i=0; i<10; i++ )
	{
		triangles.push_back( ClockCircleTrg() );
		ClockCircleTrg& trg = triangles.back();

		float s = 1.0;
		float r = 0.0;
		
		r = ofRandom( 0.0, 1.0 );
		s = ofRandom( 1.0, 1.05 );
		trg.p1.x = radius * s * sin( r * TWO_PI );
		trg.p1.y = radius * s * cos( r * TWO_PI );
		
		r = ofRandom( 0.0, 1.0 );
		s = ofRandom( 1.0, 1.05 );
		trg.p2.x = radius * s * sin( r * TWO_PI );
		trg.p2.y = radius * s * cos( r * TWO_PI );

		r = ofRandom( 0.0, 1.0 );
		s = ofRandom( 1.0, 1.05 );
		trg.p3.x = radius * s * sin( r * TWO_PI );
		trg.p3.y = radius * s * cos( r * TWO_PI );
		
		trg.rotation	= 0;
		trg.rotationInc	= ofRandom( -2, 2 );
		
//		trg.color.r = 255;
//		trg.color.g = 255;
//		trg.color.b = 255;
//		trg.color.a = 30;

		trg.color.r = ofRandom( 0, 255 );
		trg.color.g = ofRandom( 0, 255 );
		trg.color.b = ofRandom( 0, 255 );
		trg.color.a = 80;
	}
	
	//--
	
	spinDir	= ( ofRandom( 0.0, 1.0 ) > 0.5 ) ? 1 : -1;
	spinFrc	= ofRandom( 0.15, 0.35 );
	
	active	= false;
	
	joint	= NULL;
	point	= NULL;
	
	bCenterJoint	= false;
	bOuterJoint		= false;
	bLineupJoint	= false;
}

///////////////////////////////////////////////
//	JOINT.
///////////////////////////////////////////////

void ClockCircle :: createCenterJoint ()
{
	createJoint( 0.5, 0.5, ofRandom( 0, screenHeight * 0.2 ) );
	
	bCenterJoint = true;
}

void ClockCircle :: createOuterJoint ()
{
	createJoint( 0.5, 0.5, screenHeight * 0.38 );
	
	bOuterJoint = true;
}

void ClockCircle :: createLineupJoint ()
{
	createJoint( lineUpPoint.x, lineUpPoint.y, 0 );
	
	bLineupJoint = true;
}

void ClockCircle :: createJoint ( const ofPoint& p )
{
	createJoint( p.x, p.y );
}

void ClockCircle :: createJoint ( float x, float y, float length )
{
	if( hasJoint() )
		destroyJoint();
	
	b2BodyDef bd;
	bd.position.Set( ( x * screenWidth ) / OFX_BOX2D_SCALE, ( y * screenHeight ) / OFX_BOX2D_SCALE );
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
	
	jd.length = length / OFX_BOX2D_SCALE;
	
	joint = (b2DistanceJoint*)world->CreateJoint( &jd );
	
	body->WakeUp();
}

void ClockCircle :: destroyJoint ()
{
	if( hasJoint() )
	{
		world->DestroyJoint( joint );
		joint = NULL;
	
		world->DestroyBody( point );
		point = NULL;
	}
	
	bCenterJoint	= false;
	bOuterJoint		= false;
	bLineupJoint	= false;
}

bool ClockCircle :: hasJoint ()
{
	return ( point != NULL && joint != NULL );
}

bool ClockCircle :: hasCenterJoint ()	{ return bCenterJoint; }
bool ClockCircle :: hasOuterJoint  ()	{ return bOuterJoint;  }
bool ClockCircle :: hasLineupJoint ()	{ return bLineupJoint; }

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
	
	//--
	
	colorCurr.r += ( colorTrgt.r - colorCurr.r ) * 0.1;
	colorCurr.g += ( colorTrgt.g - colorCurr.g ) * 0.1;
	colorCurr.b += ( colorTrgt.b - colorCurr.b ) * 0.1;
	
	//--
	
	for( int i=0; i<triangles.size(); i++ )
	{
		ClockCircleTrg& trg = triangles[ i ];
		trg.rotation += trg.rotationInc;
	}
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
	
	drawCircles();
//	drawTriangles();
}

void ClockCircle :: drawCircles ()
{
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

void ClockCircle :: drawTriangles ()
{
	glPushMatrix();
	glTranslatef( getPosition().x, getPosition().y, 0 );
	
	ofEnableAlphaBlending();
	
//	glBlendFunc( GL_ONE, GL_ONE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	
	for( int i=0; i<triangles.size(); i++ )
	{
		ClockCircleTrg& trg = triangles[ i ];
		
		glPushMatrix();
		glRotatef( trg.rotation, 0, 0, 1 );
		
		ofSetColor( trg.color.r, trg.color.g, trg.color.b, trg.color.a );
		ofTriangle( trg.p1.x, trg.p1.y, trg.p2.x, trg.p2.y, trg.p3.x, trg.p3.y );
		
		glPopMatrix();
	}
	
	glBlendFunc( GL_ONE, GL_ZERO );
	
	ofDisableAlphaBlending();
	
	glPopMatrix();
}