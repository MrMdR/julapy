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
	boidsNum = boidsNumRevised = 30;
	
	addBoids( boidsNum );
	
	//--
	
	boidSeperationWeight	= 1.8;
	boidAlignmentWeight		= 1.0;
	boidCohesionWeight		= 1.0;
	
	boidSeparationDist		= 25.0;
	boidPerception			= 50.0;
	boidMaxSpeed			= 10.0;
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

void Flock :: setContainer	( ofRectangle &rect )
{
	if
	(
		containerRect.x			== rect.x		&&
		containerRect.y			== rect.y		&&
		containerRect.width		== rect.width	&&
		containerRect.height	== rect.height
	)
	{
		return;
	}
	
	containerRect.x			= rect.x;
	containerRect.y			= rect.y;
	containerRect.width		= rect.width;
	containerRect.height	= rect.height;
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.setContainer( containerRect );
	}
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
	
	updateFood();
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
	
	for( int i=0; i<obstacles.size(); i++ )
	{
		BoidObstacle &obs = obstacles[ i ];
		
		forces.push_back( BoidForce( obs.x, obs.y, obs.size, obs.reach, obs.magnitude ) );
	}
	
	for( int i=0; i<blobs->size(); i++ )
	{
		ofxCvBlob &blob = blobs->at( i );
		
		int t = blob.pts.size();
		
		for( int j=0; j<t; j++ )
		{
			ofPoint &p1 = blob.pts[ j ];
			
			forces.push_back( BoidForce( p1.x, p1.y, 40, 40, -20 ) );
		}
	}
}

void Flock :: updateFood ()
{
	for( int i=0; i<foods.size(); i++ )
	{
		bool bRemove = false;
		
		BoidFood &food = foods[ i ];
		
		for( int j=0; j<boids.size(); j++ )
		{
			Boid &boid = boids[ j ];
			
			float dist = ofDist( food.x, food.y, boid.pos.x, boid.pos.y );
			float area = food.size + boid.size;
			
			if( dist < area )
			{
				food.size -= 0.05;
				
				if( food.size < 5 )
				{
					bRemove = true;
					
					break;
				}
			}
		}
		
		if( bRemove )
		{
			foods.erase( foods.begin() + i );
			--i;
		}
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
	drawObstacles();

	ofNoFill();
	ofEnableSmoothing();

	drawBoids();
	drawMice();
	drawFood();
	drawObstacles();
	
	ofDisableSmoothing();
}

void Flock :: drawBoids ()
{
	ofSetColor( 255, 255, 255, 255 );
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.draw();
		boid.drawTrail();
		boid.drawTrailFill();
		boid.drawDebug();
	}
}

void Flock :: drawMice ()
{
	ofEnableAlphaBlending();
	
	ofNoFill();
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
	ofNoFill();
	
	for( int i=0; i<foods.size(); i++ )
	{
		BoidFood &food = foods[ i ];
		food.draw();
	}
}

void Flock :: drawObstacles ()
{
	ofNoFill();
	
	for( int i=0; i<obstacles.size(); i++ )
	{
		BoidObstacle &obs = obstacles[ i ];
		obs.draw();
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

void Flock :: addObstacle ( int x, int y )
{
	float obstacleSize;
	obstacleSize = ofRandom( 20, 50 );
	
	obstacles.push_back( BoidObstacle( x, y, obstacleSize, obstacleSize * 1.5, -20 ) );
}

void Flock :: addMouse ( int x, int y )
{
	bMouseMoved = true;
	
	mice.clear();
	mice.push_back( BoidForce( x, y, mouseReach, mouseReach, mouseForce ) );
	
	mouseCopy = mice.back();
}

void Flock :: addBlobs ( vector<ofxCvBlob> *blobsPtr )
{
	blobs = blobsPtr;
}