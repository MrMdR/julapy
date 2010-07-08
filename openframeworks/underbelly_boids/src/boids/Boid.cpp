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
	perception		= 50;
	maxSpeed		= 15.0;
	maxForce		= 1.0;
	
	separationWeight	= 1.8;
	alignmentWeight		= 1.0;
	cohesionWeight		= 1.0;
	
	bContain				= true;
	containerRect.x			= 0.0;
	containerRect.y			= 0.0;
	containerRect.width		= 1.0;
	containerRect.height	= 1.0;
	
	boids			= NULL;
}

Boid :: ~Boid()
{
	boids	= NULL;
	forces	= NULL;
	foods	= NULL;
}

/////////////////////////////////////////////
//	SETTERS.
/////////////////////////////////////////////

void Boid :: setBoids ( vector<Boid> *boidsPtr )
{
	boids = boidsPtr;
}

void Boid :: setForces ( vector<BoidForce> *forcesPtr )
{
	forces = forcesPtr;
}

void Boid :: setFoods ( vector<BoidFood> *foodsPtr )
{
	foods = foodsPtr;
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
	ofxVec2f sep = separate( boids );			//-- flocking forces.
	ofxVec2f ali = align( boids );
	ofxVec2f coh = cohesion( boids );
	
	sep *= separationWeight;
	ali *= alignmentWeight;
	coh *= cohesionWeight;
	
	ofxVec2f ptf;								//-- point forces.
	
	for( int i=0; i<forces->size(); i++ )
	{
		BoidForce &force = forces->at( i );
		ptf += pointForce( force.x, force.y, force.reach, force.magnitude );
	}
	
	accNew		= 0;
	accNew		+= sep;
	accNew		+= ali;
	accNew		+= coh;
	accNew		+= ptf;
}

void Boid :: update_vel ()
{
	velNew		+= accNew;
	velNew.limit( maxSpeed );
}

void Boid :: update_pos ()
{
	if( bContain )
	{
		ofRectangle r;
		r.x			= containerRect.x * ofGetWidth();
		r.y			= containerRect.y * ofGetHeight();
		r.width		= containerRect.width  * ofGetWidth();
		r.height	= containerRect.height * ofGetHeight();
		
		float efficiency = .03;
//		efficiency ~ 10 : boids immediately rejected
//		efficiency ~ .1 : boids slowly change direction
		
		ofxVec2f v;
		if( posNew.x <= r.x + perception )				v.x = ( r.x + perception - posNew.x ) * efficiency;
		if( posNew.x >= r.x + r.width  - perception )	v.x = ( r.x + r.width  - perception - posNew.x ) * efficiency;
		if( posNew.y <= r.y + perception)				v.y = ( r.y + perception - posNew.y ) * efficiency;
		if( posNew.y >= r.y + r.height - perception )	v.y = ( r.y + r.height - perception - posNew.y ) * efficiency;
		
		velNew += v;
	}
	else
	{
		if( posNew.x < -size ) posNew.x = ofGetWidth()  + size;
		if( posNew.y < -size ) posNew.y = ofGetHeight() + size;
		if( posNew.x > ofGetWidth()  + size ) posNew.x = -size;
		if( posNew.y > ofGetHeight() + size ) posNew.y = -size;
	}
	
	//--

	posNew += velNew;
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

	steer = reynoldsLimit( steer );
	
    return steer;
}

//--

ofxVec2f Boid :: align ( vector<Boid> *boids )
{
	ofxVec2f steer;
	int count = 0;
	
	for( int i=0; i<boids->size(); i++ )
	{
		Boid &boid = boids->at( i );
		
		float dist;
		dist = ofDist( pos.x, pos.y, boid.pos.x, boid.pos.y );
		
		if( ( dist > 0 ) && ( dist < perception ) )
		{
			steer += boid.vel;
			count++;
		}
	}
    
	if( count > 0 )
	{
		steer /= (float)count;
	}
	
	steer = reynoldsLimit( steer );
	
    return steer;
}

//--

ofxVec2f Boid :: cohesion( vector<Boid> *boids )
{
    ofxVec2f center;
    int count = 0;
	
	for( int i=0; i<boids->size(); i++ )
	{
		Boid &boid = boids->at( i );
		
		float dist;
		dist = ofDist( pos.x, pos.y, boid.pos.x, boid.pos.y );
		
		if( ( dist > 0 ) && ( dist < perception ) )
		{
			center += boid.pos;			// add position.
			count++;
		}
	}
    
	ofxVec2f steer;
	
	if( count > 0 )
	{
		center /= (float)count;			// old.
		steer = center - pos;
		
		steer = reynoldsLimit( steer );
    }
	
	return steer;
}

//--

ofxVec2f Boid :: reynoldsLimit ( ofxVec2f &desired )		// implement Reynolds :: steering = desired minus velocity
{
	ofxVec2f steer;
	
	if( desired.length() > 0 )		
	{
		desired.normalize();				// work out desired direction.
		desired *= maxSpeed;				// work out the fastest desired speed in that direction.
		
		steer = desired - vel;				// implement Reynolds :: steering = desired minus velocity
		steer.limit( maxForce );			// limit to maximum steering force
    }
	
	return steer;
}

//--

ofxVec2f Boid :: pointForce( float x, float y, float reach, float magnitude )
{
	ofxVec2f v;
	ofxVec2f p1;
	ofxVec2f p2;
	
	p1.set( x, y );
	p2.set( pos );
	v = p1 - p2;
	
	float dist;
	dist = v.length();
	
	if( dist < reach )
	{
		v.normalize();
		
		float p;
		p = ( reach - dist ) / reach;		// between 0 - 1.
		
		v *= p;
		v *= magnitude;
		
		return v;
	}
	
	v.set( 0, 0 );
	return v;
}

