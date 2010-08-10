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
	digitIndex	= 0;
	
	radius		= r;
	colorHex	= c;
	rotation	= 0;
	
	colorInit.r	= colorCurr.r = colorTrgt.r = ( c >> 16 ) & 0xff;
	colorInit.g	= colorCurr.g = colorTrgt.g = ( c >> 8  ) & 0xff;
	colorInit.b	= colorCurr.b = colorTrgt.b = ( c >> 0  ) & 0xff;
	
	forceScale	= 1.0;
	
	tex				= NULL;
	texAnim			= NULL;
	texAnimIndex	= 0;
	
//	lineLength		= ofRandom( 0.08, 0.15 );
	lineLength		= 0.02;
//	lineLength		= 0.01;
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
	
	screenMinLength	= MIN( screenWidth, screenHeight );
	screenMaxLength	= MAX( screenWidth, screenHeight );
}

void ClockCircle :: setTexture ( ofTexture* tex )
{
	this->tex = tex;
}

void ClockCircle :: setTextureAnim ( vector<ofTexture*>* tex )
{
	texAnim = tex;
}

void ClockCircle :: setForceScale ( float scale )
{
	forceScale = scale;
}

void ClockCircle :: setDigitIndex ( int index )
{
	digitIndex = index;
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
	
	if( false )
	{
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
			
			trg.color.r = ofRandom( 0, 255 );
			trg.color.g = ofRandom( 0, 255 );
			trg.color.b = ofRandom( 0, 255 );
			trg.color.a = 80;
		}
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
	
	centerPoint.x = screenWidth * 0.5;
	centerPoint.y = screenHeight * 0.5;
}

void ClockCircle :: setup( b2World* b2dworld, float x, float y, float size, bool isFixed )
{
	if( b2dworld == NULL )
	{
		ofLog( OF_LOG_NOTICE, "- must have a valid world -" );
		return;
	}
	
	world				= b2dworld;
	circle.radius		= size/OFX_BOX2D_SCALE;
	bIsFixed			= isFixed;
	
	if( isFixed )
	{
		circle.density		= 0;
		circle.restitution  = 0;
		circle.friction		= 0;
	}
	else
	{
		circle.density		= mass;
		circle.restitution  = bounce;
		circle.friction		= friction;
	}
	
	bodyDef.position.Set( x / OFX_BOX2D_SCALE, y / OFX_BOX2D_SCALE );
	
	body = world->CreateBody( &bodyDef );
	
	if( body )
	{
		body->SetLinearVelocity( b2Vec2( 0.0, 0.0 ) );
		body->CreateShape( &circle );
		
		if( true )
		{
			b2Vec2 localPos;
			localPos = body->GetWorldCenter();
			
			b2MassData massData;
			massData.mass	= 3.0 * forceScale;
			massData.center = localPos;
			massData.I		= 0;
			
//			massData.mass	= mass * PI * circle.radius * circle.radius;
//			massData.center	= localPos;
//			massData.I		= massData.mass * ( 0.5f * circle.radius * circle.radius + b2Dot( localPos, localPos ) );
			
			body->SetMass( &massData );
		}
		else
		{
			body->SetMassFromShapes();
		}
	}
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
	rotation = getRotation();
	
	if( tex == NULL )
	{
		const b2XForm& xf	= body->GetXForm();
		b2Vec2	center		= body->GetPosition();
		b2Vec2	axis		= xf.R.col1;
		b2Vec2	p			= center + ( radius * 0.6 ) / OFX_BOX2D_SCALE * axis;
		
		eye.x = p.x * OFX_BOX2D_SCALE;
		eye.y = p.y * OFX_BOX2D_SCALE;
	}
	
	//--
	
	colorTrgt.r += ( colorInit.r - colorTrgt.r ) * 0.05;
	colorTrgt.g += ( colorInit.g - colorTrgt.g ) * 0.05;
	colorTrgt.b += ( colorInit.b - colorTrgt.b ) * 0.05;
	
	colorCurr.r += ( colorTrgt.r - colorCurr.r ) * 0.3;
	colorCurr.g += ( colorTrgt.g - colorCurr.g ) * 0.3;
	colorCurr.b += ( colorTrgt.b - colorCurr.b ) * 0.3;
	
	//--
	
	if( !active )
	{
		circlePoint.set( body->GetPosition().x * OFX_BOX2D_SCALE, body->GetPosition().y * OFX_BOX2D_SCALE );
		
		linePoint1.set( circlePoint - centerPoint );
		linePoint1.normalize();
		linePoint1 *= radius + screenMinLength * 0.01;
		linePoint1 += circlePoint;
		
		linePoint2.set( circlePoint - centerPoint );
		linePoint2.normalize();
		linePoint2 *= radius + screenMinLength * 0.015;
		linePoint2 += circlePoint;
		
		float linePoint3x;
		linePoint3x = linePoint2.x;
		linePoint3x += ( circlePoint.x < centerPoint.x ) ? ( -screenMinLength * lineLength ) : ( screenMinLength * lineLength );
		
		linePoint3.x += ( linePoint3x  - linePoint3.x ) * 0.1;
		linePoint3.y = linePoint2.y;
		
		linePoint4.x = linePoint3.x;
		linePoint4.x += ( circlePoint.x < centerPoint.x ) ? ( -screenMinLength * 0.01 ) : ( screenMinLength * 0.01 );
		linePoint4.y = linePoint2.y;
	}
	
	//--
	
	for( int i=0; i<triangles.size(); i++ )
	{
		ClockCircleTrg& trg = triangles[ i ];
		trg.rotation += trg.rotationInc;
	}
	
	//--
	
	if( texAnim != NULL )
	{
		if( ++texAnimIndex > texAnim->size() - 1 )
		{
			texAnimIndex = 0;
		}
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
	
	if ( tex != NULL )
	{
		drawTexture();
		
		if( texAnim != NULL )
		{
			drawTextureAnim();
		}
	}
	else
	{
		drawCircles();
	}
	
//	drawTriangles();
}

void ClockCircle :: drawCircles ()
{
	ofSetColor( colorCurr.r, colorCurr.g, colorCurr.b );
	
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

void ClockCircle :: drawTexture ()
{
	ofEnableAlphaBlending();
//	ofSetColor( 255, 255, 255 );
	ofSetColor( colorCurr.r, colorCurr.g, colorCurr.b, 220 );
	
	glPushMatrix();
	glTranslatef( getPosition().x, getPosition().y, 0 );
	
	glRotatef( rotation, 0, 0, 1 );
	
	tex->draw( -radius, -radius, radius * 2, radius * 2 );
	
	glPopMatrix();
	
	ofDisableAlphaBlending();
}

void ClockCircle :: drawTextureAnim ()
{
	ofEnableAlphaBlending();
//	ofSetColor( 0, 0, 0, 100 );
	
	float colScale = 0.6;
	int r = (int)( colorCurr.r * colScale );
	int g = (int)( colorCurr.g * colScale );
	int b = (int)( colorCurr.b * colScale );
	ofSetColor( r, g, b, 255 );
	
	glPushMatrix();
	glTranslatef( getPosition().x, getPosition().y, 0 );
	
	glRotatef( rotation, 0, 0, 1 );

	texAnim->at( texAnimIndex )->draw( -radius, -radius, radius * 2, radius * 2 );
	
	glPopMatrix();
	
	ofDisableAlphaBlending();
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

void ClockCircle :: drawInfoLine ()
{
	ofEnableAlphaBlending();
	
	ofNoFill();
	ofSetColor( 0, 0, 0, 20 );
	
	ofLine( linePoint1.x, linePoint1.y, linePoint2.x, linePoint2.y );
	ofLine( linePoint2.x, linePoint2.y, linePoint3.x, linePoint3.y );
	
	ofDisableAlphaBlending();
}