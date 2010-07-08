/*
 *  Flock.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Flock.h"


Flock :: Flock()
{
	//
}

Flock :: ~Flock ()
{
	//
}

/////////////////////////////////////////////
//	INIT.
/////////////////////////////////////////////

void Flock :: init ()
{
	boidsNum = 50;
	
	for( int i=0; i<boidsNum; i++ )
	{
		boids.push_back( Boid() );
		
		Boid &boid = boids.back();
		boid.setBoids( &boids );
		boid.setForces( &forces );
		boid.setPosition
		(
			ofRandom( 0, ofGetWidth()  ),
			ofRandom( 0, ofGetHeight() )
		);
		boid.setVelocity
		(
			ofRandom( -2, 2 ),
			ofRandom( -2, 2 )
		);
		
		boid.trailCol.push_back( 0xFFFFFF );
	}
	
	//--
	
	Boid &boid = boids[ 0 ];		// store the boid group setting based on the first boid.
	
	boidSeperationWeight	= boid.separationWeight;
	boidAlignmentWeight		= boid.alignmentWeight;
	boidCohesionWeight		= boid.cohesionWeight;
	
	boidSeparationDist		= boid.separationDist;
	boidPerception			= boid.perception;
	boidMaxSpeed			= boid.maxSpeed;
	boidMaxForce			= boid.maxForce;
	
	mouseReach				= 70;
	mouseForce				= -10;
	bMouseMoved				= false;
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void Flock :: update ()
{
	updateForces();
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		
		boid.separationWeight	= boidSeperationWeight;
		boid.alignmentWeight	= boidAlignmentWeight;
		boid.cohesionWeight		= boidCohesionWeight;
		
		boid.separationDist		= boidSeparationDist;
		boid.perception			= boidPerception;
		boid.maxSpeed			= boidMaxSpeed;
		boid.maxForce			= boidMaxForce;
	}
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.update_acc();
		boid.update_vel();
		boid.update_pos();
	}
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.update_final();
	}
}

void Flock :: updateForces ()
{
	clearForces();
	
	if( bMouseMoved )
	{
		bMouseMoved = false;
		
		addForce( mousePoint.x, mousePoint.y, mouseReach, mouseForce );
	}
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void Flock :: draw ()
{
	drawBoids();
	drawForces();
	drawFood();
}

void Flock :: drawBoids ()
{
	ofFill();
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.draw();
	}
	
	ofNoFill();
	ofEnableSmoothing();
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.draw();
	}
	
	ofDisableSmoothing();
}

void Flock :: drawForces ()
{
	ofEnableAlphaBlending();

	ofFill();
	ofSetColor( 100, 100, 100, 128 );
	
	for( int i=0; i<forces.size(); i++ )
	{
		BoidForce &force = forces[ i ];
		
		ofCircle( force.x, force.y, force.reach );
	}
	
	ofDisableAlphaBlending();
}

void Flock :: drawFood ()
{
	ofEnableAlphaBlending();
	
	for( int i=0; i<foods.size(); i++ )
	{
		foods[ i ].draw();
	}
	
	ofDisableAlphaBlending();
}

/////////////////////////////////////////////
//	FORCES.
/////////////////////////////////////////////

void Flock :: addForce ( float x, float y, float reach, float magnitude )
{
	forces.push_back( BoidForce() );
	
	BoidForce &force = forces.back();
	force.x			= x;
	force.y			= y;
	force.point.set( x, y, 0 );
	force.reach		= reach;
	force.magnitude	= magnitude;
}

void Flock :: clearForces ()
{
	forces.clear();
}

/////////////////////////////////////////////
//	FOOD.
/////////////////////////////////////////////

void Flock :: addFood ( int x, int y )
{
	foods.push_back( BoidFood() );
	
	BoidFood &food = foods.back();
	food.setPosition( x, y );
	food.setRadius( ofRandom( 10, 30 ) );
}

/////////////////////////////////////////////
//	MOUSE.
/////////////////////////////////////////////

void Flock :: addMouse ( int x, int y )
{
	bMouseMoved = true;
	
	mousePoint.x = x;
	mousePoint.y = y;
}
