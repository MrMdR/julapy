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
	boidsNum = boidsNumRevised = 100;
	
	addBoids( boidsNum );
	
	//--
	
	boidSeperationWeight	= 1.8;
	boidAlignmentWeight		= 1.0;
	boidCohesionWeight		= 1.0;
	
	boidSeparationDist		= 25.0;
	boidPerception			= 50.0;
	boidMaxSpeed			= 15.0;
	boidMaxForce			= 1.0;
	
	bMouseMoved				= false;
	mouseReach				= 70;
	mouseForce				= -10;
}

void Flock :: addBoids ( int num )
{
	for( int i=0; i<num; i++ )
	{
		boids.push_back( Boid() );
		
		addBoid( boids.back() );
	}
}

void Flock :: addBoid ( Boid &boid )
{
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

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void Flock :: update ()
{
	updateForces();
	
	if( boidsNum != boidsNumRevised )						//-- need to add or remove boids.
	{
		if( boidsNumRevised > boidsNum )		// add.
		{
			int num = boidsNumRevised - boidsNum;
			addBoids( num );
		}
		
		if( boidsNumRevised < boidsNum )		// remove.
		{
			int i = boidsNumRevised;
			int t = boidsNum;
			
			boids.erase( boids.begin() + i, boids.begin() + t );
		}
		
		boidsNum = boidsNumRevised;
	}
	
	for( int i=0; i<boids.size(); i++ )						//-- update boid variables.
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
	
	for( int i=0; i<boids.size(); i++ )						//-- update boid movement and save as new value.
	{
		Boid &boid = boids[ i ];
		boid.update_acc();
		boid.update_vel();
		boid.update_pos();
	}
	
	for( int i=0; i<boids.size(); i++ )						//-- update boid with saved boid movement.
	{
		Boid &boid = boids[ i ];
		boid.update_final();
	}
}

void Flock :: updateForces ()
{
	forces.clear();
	
	if( mice.size() > 0 )
	{
		forces.push_back( mice[ 0 ] );
		
		mice.clear();
	}
	
	for( int i=0; i<foods.size(); i++ )
	{
		BoidFood &food = foods.at( i );
		
		forces.push_back( food.getForceOne() );
		forces.push_back( food.getForceTwo() );
	}
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void Flock :: draw ()
{
	ofFill();
	ofDisableSmoothing();
	
	drawBoids();
	drawMice();
	drawFood();

	ofNoFill();
	ofEnableSmoothing();

	drawBoids();
	drawMice();
	drawFood();
	
	ofDisableSmoothing();
}

void Flock :: drawBoids ()
{
	ofSetColor( 255, 255, 255, 255 );
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.draw();
	}
}

void Flock :: drawMice ()
{
	ofEnableAlphaBlending();
	
	ofSetColor( 255, 0, 255, 128 );
	
	for( int i=0; i<mice.size(); i++ )
	{
		BoidForce &mouse = mice[ i ];
		
		ofCircle( mouse.x, mouse.y, mouse.reach );
	}
	
	if( bMouseMoved )
	{
		bMouseMoved = false;
		
		ofCircle( mouseCopy.x, mouseCopy.y, mouseCopy.reach );
	}
	
	ofDisableAlphaBlending();
}

void Flock :: drawFood ()
{
	for( int i=0; i<foods.size(); i++ )
	{
		BoidFood &food = foods[ i ];
		food.draw();
	}
}

/////////////////////////////////////////////
//	FORCES.
/////////////////////////////////////////////

void Flock :: addFood ( int x, int y )
{
	float foodSize;
	foodSize = ofRandom( 10, 30 );
	
	foods.push_back( BoidFood( x, y, foodSize, foodSize, 10 ) );
}

void Flock :: addMouse ( int x, int y )
{
	bMouseMoved = true;
	
	mice.clear();
	mice.push_back( BoidForce( x, y, mouseReach, mouseReach, mouseForce ) );
	
	mouseCopy = mice.back();
}
