/*
 *  ColorPhysics.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorPhysics.h"

ColorPhysics :: ColorPhysics ()
{
	box2d = NULL;
	
	setGravity( 0, 0 );
}

ColorPhysics :: ~ColorPhysics ()
{

}

void ColorPhysics :: setScreen ( ofxScreen screen )
{
	this->screen = screen;
}

void ColorPhysics :: setGravity	( float gx, float gy )
{
	gravity.x = MIN( 1.0, MAX( -1.0, gx ) );
	gravity.y = MIN( 1.0, MAX( -1.0, gy ) );
	
	if( box2d != NULL )
	{
		box2d->setGravity( gravity.x * 10, gravity.y * 10 );
	}
}

///////////////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////////////

void ColorPhysics :: setup ()
{
	box2d = new ofxBox2d();
	box2d->init();
	box2d->setGravity( 0, 0 );
	box2d->setFPS( 30.0 );
	box2d->registerGrabbing();
//	box2d->getWorld()->SetContactListener( &contactListener );
	
	createBounds();
	createCircles();
}

void ColorPhysics :: createBounds ()
{
	b2BodyDef bd;
	bd.position.Set( 0, 0 );
	ground = box2d->ground = box2d->world->CreateBody( &bd );
	
	b2PolygonDef sd;
	sd.filter.groupIndex = 1;
	sd.density		= 0.0f;
	sd.restitution	= 0.0f;
	sd.friction		= 0.6;
	
	float thick		= 0.02 * screen.screenHeight;
	
	int w = screen.screenWidth;
	int h = screen.screenHeight;
	
	sd.SetAsBox		//-- right		( float32 hx, float32 hy, const b2Vec2& center, float32 angle )
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2,
		b2Vec2( ( w + thick ) / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape( &sd );
	
	sd.SetAsBox		//-- left
	(
		thick / OFX_BOX2D_SCALE,
		( h / OFX_BOX2D_SCALE ) / 2, 
		b2Vec2( -thick / OFX_BOX2D_SCALE, ( h / OFX_BOX2D_SCALE ) / 2 ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- top
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, -thick /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
	
	sd.SetAsBox		//-- bottom
	(
		( w / OFX_BOX2D_SCALE ) / 2,
		thick / OFX_BOX2D_SCALE,
		b2Vec2( ( w / OFX_BOX2D_SCALE ) / 2, ( h + thick ) /OFX_BOX2D_SCALE ),
		0.0
	);
	ground->CreateShape(&sd);
}

void ColorPhysics :: createCircles ()
{
	int t = 20;
	for( int i=0; i<t; i++ )
	{
		ColorCircle* circle;
		circle = new ColorCircle();
		
		//-- define line up point.
		
		float area		= ofRandom( 0.001, 0.005 );
		float radius	= areaToRadius( area );
		float rx = radius / (float)screen.screenWidth;
		float ry = radius / (float)screen.screenHeight;
		
		circle->setScreen( screen );
		circle->init();		// do things here before creating the circle in box2d.
		
		//-- define circles physics.
		
		float mass		= 3.0;
		float bounce	= 0.53;
		float friction	= 0.1;
		
		circle->enableGravity( true );
		circle->setPhysics( mass, bounce, friction );
		circle->setup( box2d->getWorld(), ofRandom( 0, screen.screenWidth ), ofRandom( 0, screen.screenHeight ), radius, false );
		circle->setRotationFriction( 1.0 );
		circle->setDamping( 1.0 );
		circle->body->AllowSleeping( false );
		
		//-- add to vectors.
		
		circles.push_back( circle );
	}
}

float ColorPhysics :: areaToRadius ( float area )
{
	float r;
	r = sqrt( area * screen.screenArea );
	r *= 0.5;
	
	return r;
}

///////////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////////

void ColorPhysics :: update ()
{
	for( int i=0; i<circles.size(); i++ )
	{
		ColorCircle& circle = *circles[ i ];
		
		circle.update();
	}
	
	box2d->update();
}

///////////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////////

void ColorPhysics :: draw ()
{
	box2d->draw();
	
	for( int i=0; i<circles.size(); i++ )
	{
		ColorCircle& circle = *circles[ i ];
		
		circle.draw();
	}
}