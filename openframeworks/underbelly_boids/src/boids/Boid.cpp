/*
 *  Boid.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 5/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Boid.h"

Boid :: Boid()
{
	size			= 5.0;
	separationDist	= 25.0;
	neighbourDist	= 50.0;
	maxSpeed		= 10.0;
	maxForce		= 0.1;
	
	separationWeight	= 1.5;
	alignmentWeight		= 1.0;
	cohesionWeight		= 1.0;
	
	boids			= NULL;
}

Boid :: ~Boid()
{
	//
}

/////////////////////////////////////////////
//	SETTERS.
/////////////////////////////////////////////

void Boid :: setBoids ( vector<Boid> *boidsPtr )
{
	boids = boidsPtr;
}

void Boid :: setPosition ( float x, float y )
{
	pos.x = posNew.x = x;
	pos.y = posNew.y = y;
}

void Boid :: setVelocity ( float x, float y )
{
	vel.x = velNew.x = x;
	vel.y = velNew.y = y;
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void Boid :: update_acc ()
{
	ofxVec2f sep = separate( boids );
	ofxVec2f ali = align( boids );
	ofxVec2f coh = cohesion( boids );
	
	sep *= separationWeight;
	ali *= alignmentWeight;
	coh *= cohesionWeight;
	
	accNew		= 0;
	accNew		+= sep;
	accNew		+= ali;
	accNew		+= coh;
}

void Boid :: update_vel ()
{
	velNew		+= accNew;
	velNew.x	= ofClamp( velNew.x, -maxSpeed, maxSpeed );
	velNew.y	= ofClamp( velNew.y, -maxSpeed, maxSpeed );
}

void Boid :: update_pos ()
{
	posNew		+= velNew;
	
	if( posNew.x < -size ) posNew.x = ofGetWidth()  + size;
	if( posNew.y < -size ) posNew.y = ofGetHeight() + size;
	if( posNew.x > ofGetWidth()  + size ) posNew.x = -size;
	if( posNew.y > ofGetHeight() + size ) posNew.y = -size;
}

void Boid :: update_final ()
{
	pos = posNew;
	vel = velNew;
	acc = accNew;
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void Boid :: draw ()
{
	int c = trailCol.back();
	
	ofSetColor( c );
	ofCircle( pos.x, pos.y, size );
}

/////////////////////////////////////////////
//	BEHAVIOUR.
/////////////////////////////////////////////

/*
 *
 * Separation
 * Method checks for nearby boids and steers away
 *
 */

ofxVec2f Boid :: separate( vector<Boid> *boids )
{
	ofxVec2f steer;
	int count = 0;
	
    for( int i=0; i<boids->size(); i++ )		// For every boid in the system, check if it's too close
	{
		Boid &boid = boids->at( i );
		
		float dist;
		dist = ofDist( pos.x, pos.y, boid.pos.x, boid.pos.y );
		
		if( ( dist > 0 ) && ( dist < separationDist ) )		// 0 when you are yourself
		{
			ofxVec2f diff;				// Calculate vector pointing away from neighbor
			diff = pos - boid.pos;
			diff.normalize();
			diff /= dist;				// Weight by distance
			
			steer += diff;
			
			count++;
		}
    }
	
    if( count > 0 )						// Average -- divide by how many
	{
		steer /= (float)count;
    }

	float mag;
	mag = steer.length();
	
    if( mag > 0 )						// Implement Reynolds: Steering = Desired - Velocity
	{
		steer.normalize();
		steer *= maxSpeed;
		steer -= vel;
		steer.limit( maxForce );
	}
	
    return steer;
}

/*
 *
 * Alignment
 * For every nearby boid in the system, calculate the average velocity
 *
 */

ofxVec2f Boid :: align ( vector<Boid> *boids )
{
	ofxVec2f steer;
	int count = 0;
	
	for( int i=0; i<boids->size(); i++ )
	{
		Boid &boid = boids->at( i );
		
		float dist;
		dist = ofDist( pos.x, pos.y, boid.pos.x, boid.pos.y );
		
		if( ( dist > 0 ) && ( dist < neighbourDist ) )
		{
			steer += boid.vel;
			count++;
		}
	}
    
	if( count > 0 )
	{
		steer /= (float)count;
	}
	
	float mag;
	mag = steer.length();
	
    if( mag > 0 )		// Implement Reynolds: Steering = Desired - Velocity
	{
		steer.normalize();
		steer *= maxSpeed;
		steer -= vel;
		steer.limit( maxForce );
    }
	
    return steer;
}

/*
 *
 * Cohesion
 * For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
 *
 */

ofxVec2f Boid :: cohesion( vector<Boid> *boids )
{
    ofxVec2f sum;
    int count = 0;
	
	for( int i=0; i<boids->size(); i++ )
	{
		Boid &boid = boids->at( i );
		
		float dist;
		dist = ofDist( pos.x, pos.y, boid.pos.x, boid.pos.y );
		
		if( ( dist > 0 ) && ( dist < neighbourDist ) )
		{
			sum += boid.vel;;						// Add location
			count++;
		}
	}
    
	if( count > 0 ) 
	{
		sum /= (float)count;
		return moveTo( sum );						// Steer towards the location
    }
	
	return sum;
}

/*
 *
 * A method that calculates a steering vector towards a target
 * Takes a second argument, if true, it slows down as it approaches the target
 *
 */

ofxVec2f Boid :: moveTo( ofxVec2f target, bool slowdown )
{
    ofxVec2f steer;
    ofxVec2f desired;
	
	desired = target - pos;
	
	float dist;
	dist = ofDist( target.x, target.y, pos.x, pos.y );		// Distance from the target is the magnitude of the vector
    
    if( dist > 0 )											// If the distance is greater than 0, calc steering (otherwise return zero vector)
	{
		desired /= dist;									// Normalize desired
		
		if( ( slowdown ) && ( dist < 100.0f ) )				// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		{
			desired *= maxSpeed * ( dist / 100.0f );		// This damping is somewhat arbitrary
		}
		else
		{
			desired *= maxSpeed;
		}
		
		steer = desired - vel;								// Steering = Desired minus Velocity
		steer.x = ofClamp( steer.x, -maxForce, maxForce );	// Limit to maximum steering force
		steer.y = ofClamp( steer.y, -maxForce, maxForce ); 
	}
	
	return steer;
}
