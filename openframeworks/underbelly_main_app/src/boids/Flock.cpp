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
	home.x			= 1.02;
	home.y			= 0.5;
	home.size		= 50;
	home.reach		= 200;
	home.magnitude	= 20;
	
	boidSeperationWeight	= 1.8;
	boidAlignmentWeight		= 1.0;
	boidCohesionWeight		= 1.0;
	boidRandomWeight		= 0.5;
	
	boidSeparationDist		= 25.0;
	boidPerception			= 50.0;
	boidMaxSpeed			= 10.0;
	boidMaxForce			= 1.0;
	
	bMouseMoved				= false;
	mouseReach				= 70;
	mouseForce				= -10;
	
	//-- boids.
	
	boidsNum = boidsNumRevised = 30;
	
	addBoids( boidsNum );
	
	//-- queen.
	
	queen.setPosition( 0, 0 );
	queen.setVelocity( ofRandom( -2, 2 ), ofRandom( -2, 2 ) );
	queen.setBoids( &boids );
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
//		ofRandom( 0, ofGetWidth()  ),
//		ofRandom( 0, ofGetHeight() )
		0,
		0
	);
	
	boid.setVelocity
	(
		ofRandom( -2, 2 ),
		ofRandom( -2, 2 )
	);
	
	boid.setHome( home );
	
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
	
	queen.setContainer( containerRect );
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
	
	//-- update part one.
	
	for( int i=0; i<boids.size(); i++ )						
	{
		updateBoidPartOne( boids[ i ] );
	}

	queen.perception	= boidPerception * 3;
	queen.maxSpeed		= boidMaxSpeed * 0.5;
	updateBoidPartOne( queen, false );
	
	//-- update part two.
	
	for( int i=0; i<boids.size(); i++ )
	{
		updateBoidPartTwo( boids[ i ] );
	}
	
	queen.update_final();
	
	//-- yum!
	
	updateFood();
}

void Flock :: updateBoidPartOne ( Boid &boid, bool bUpdateVars )
{
	if( bUpdateVars )
	{
		boid.separationWeight	= boidSeperationWeight;		//-- update boid variables.
		boid.alignmentWeight	= boidAlignmentWeight;
		boid.cohesionWeight		= boidCohesionWeight;
		boid.randomWeight		= boidRandomWeight;
		
		boid.separationDist		= boidSeparationDist;
		boid.perception			= boidPerception;
		boid.maxSpeed			= boidMaxSpeed;
		boid.maxForce			= boidMaxForce;
	}
	
	boid.update_acc();									//-- update boid movement and save as new value.
	boid.update_vel();
	boid.update_pos();
}

void Flock :: updateBoidPartTwo ( Boid &boid )
{
	boid.update_final();
	boid.update_home();
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
	
	forces.push_back( BoidForce( queen.pos.x, queen.pos.y, 50, 50, -20 ) );
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
	drawHome();
	
	ofDisableSmoothing();
}

void Flock :: drawBoids ()
{
	for( int i=0; i<boids.size(); i++ )
	{
		ofSetColor( 0xFFFFFF );
		
		Boid &boid = boids[ i ];
		boid.draw();
		boid.drawTrail();
		boid.drawTrailFill();
		boid.drawDebug();
	}
	
	ofSetColor( 0xFF0000 );
	
	queen.draw();
	queen.drawTrail();
	queen.drawTrailFill();
	queen.drawDebug();
}

void Flock :: drawMice ()
{
	ofNoFill();
	ofSetColor( 0xFF00FF );
	
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

void Flock :: drawHome ()
{
	ofNoFill();
	ofSetColor( 0xFFFF00 );
	ofCircle( home.x * containerRect.width, home.y * containerRect.height, home.size );
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