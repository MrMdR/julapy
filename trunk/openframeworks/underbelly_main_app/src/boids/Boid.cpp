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
	randomWeight		= 1.0;
	
	trailLength			= 20;
	
	bContain				= true;
	containerRect.x			= 0.0;
	containerRect.y			= 0.0;
	containerRect.width		= ofGetWidth();
	containerRect.height	= ofGetHeight();
	
	boids			= NULL;
	forces			= NULL;
	
	bPullHome		= true;
	bIsHome			= false;
	bLeftHome		= false;
	homeInTime		= 0;
	homeInTimeout	= 0;
	homeOutTime		= 0;
	homeOutTimeout	= 0;
}

Boid :: ~Boid()
{
	boids	= NULL;
	forces	= NULL;
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

void Boid :: setHome ( BoidForce homeForce )
{
	home = homeForce;
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

void Boid :: setContainer ( ofRectangle &rect )
{
	containerRect.x			= rect.x;
	containerRect.y			= rect.y;
	containerRect.width		= rect.width;
	containerRect.height	= rect.height;
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void Boid :: update_acc ()
{
	accNew = 0;									// zero the acceleration.
	
	//-- point forces.
	
	ofxVec2f ptf;						

	if( bPullHome )
	{
		int homeX = home.x * containerRect.width;
		int homeY = home.y * containerRect.height;
		
		ptf += pointForce( homeX, homeY, home.reach, home.magnitude );
	}
	
	if( !bIsHome && forces != NULL )
	{
		for( int i=0; i<forces->size(); i++ )
		{
			BoidForce &force = forces->at( i );
			ptf += pointForce( force.x, force.y, force.reach, force.magnitude );
		}
	}
	
	accNew += ptf;
	
	//-- random motion.
	
	ofxVec2f rnd;
	rnd.set( ofRandom( -randomWeight, randomWeight ), ofRandom( -randomWeight, randomWeight ) );
	
	accNew += rnd;

	//-- flocking forces.

	if( !bIsHome && boids != NULL )
	{
		ofxVec2f sep = separate( boids );
		ofxVec2f ali = align( boids );
		ofxVec2f coh = cohesion( boids );
		
		sep *= separationWeight;
		ali *= alignmentWeight;
		coh *= cohesionWeight;
		
		accNew		+= sep;
		accNew		+= ali;
		accNew		+= coh;
	}
}

void Boid :: update_vel ()
{
	velNew		+= accNew;
	velNew.limit( maxSpeed );
}

void Boid :: update_pos ()
{
	ofRectangle r;
	r.x			= containerRect.x;
	r.y			= containerRect.y;
	r.width		= containerRect.width;
	r.height	= containerRect.height;
	
	if( bContain )
	{
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
		if( posNew.x < -size ) posNew.x = r.width  + size;
		if( posNew.y < -size ) posNew.y = r.height + size;
		if( posNew.x > r.width  + size ) posNew.x = -size;
		if( posNew.y > r.height + size ) posNew.y = -size;
	}
	
	//--

	posNew += velNew;
}

void Boid :: update_final ()
{
	pos = posNew;
	vel = velNew;
	acc = accNew;
	
	//-- add position to trail.
	
	trailPos.push_back( ofxVec2f() );
	ofxVec2f &tp = trailPos.back();
	tp.set( pos );
	
	//-- add direction of trail.
	
	if( trailPos.size() > 1 )
	{
		int t = trailPos.size();
		
		dir  = trailPos[ t - 1 ];
		dir -= trailPos[ t - 2 ];
		dir.normalize();
		
		perp.set( dir );
		perp.perpendicular();
		
		trailDir.push_back( perp );
		
		//-- if max numbe is reached, start removing old trail positions.
		
		if( trailPos.size() > trailLength )				
		{
			int t = trailPos.size() - trailLength;
			
			trailPos.erase( trailPos.begin(), trailPos.begin() + t );
			trailDir.erase( trailDir.begin(), trailDir.begin() + t );
		}
	}
	else
	{
		trailDir.push_back( ofxVec2f() );
	}
}

void Boid :: update_home ()
{
	int msElapsed;
	msElapsed = ofGetElapsedTimeMillis();
	
	if( bPullHome && !bIsHome )
	{
		float homeX = home.x * containerRect.width;
		float homeY = home.y * containerRect.height;
		float dist	= ofDist( pos.x, pos.y, homeX, homeY );
		
		if( dist < home.size )
		{
			bIsHome			= true;
			
			homeInTime		= msElapsed;
			homeInTimeout	= (int)( ofRandom( 1000, 2000 ) );
		}
	}
	
	if( bIsHome )
	{
		int t;
		t = msElapsed - homeInTime;
		
		if( t > homeInTimeout )
		{
			bPullHome	= false;
			bIsHome		= false;
			bLeftHome	= true;
			
			homeOutTime		= msElapsed;
			homeOutTimeout	= (int)( ofRandom( 3000, 5000 ) );
		}
	}
	
	if( bLeftHome )
	{
		int t;
		t = msElapsed - homeOutTime;
		
		if( t > homeOutTimeout )
		{
			bPullHome	= true;
			bIsHome		= false;
			bLeftHome	= false;
		}
	}
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void Boid :: draw ()
{
	ofFill();
	ofCircle( pos.x, pos.y, size );
}

void Boid :: drawDebug ()
{
	int line = 14;
	
	ofNoFill();
	
	ofSetColor( 0xFF0000 );
	ofLine( pos.x, pos.y, pos.x + dir.x * line, pos.y + dir.y * line );
	
	ofSetColor( 0x00FF00 );
	ofLine( pos.x, pos.y, pos.x + perp.x * line, pos.y + perp.y * line );
}

void Boid :: drawTrail ()
{
	ofNoFill();
	
	if( trailPos.size() < 2 )
		return;
	
	for( int i=0; i<trailPos.size() - 1; i++ )
	{
		ofxVec2f &p1 = trailPos[ i ];
		ofxVec2f &p2 = trailPos[ i + 1 ];
		
		ofLine( p1.x, p1.y, p2.x, p2.y );
	}
}

void Boid :: drawTrailFill ()
{
	ofNoFill();
	
	int t = trailPos.size() - 1;
	
	for( int i=0; i<t; i++ )
	{
		ofxVec2f &p1 = trailPos[ i ];
		ofxVec2f &p2 = trailPos[ i + 1 ];
		
		ofxVec2f &d1 = trailDir[ i ];
		ofxVec2f &d2 = trailDir[ i + 1 ];

		float p		= i / (float)t;
		float s		= 1 - cos( PI * 0.5 * p );
		float w		= 10 * s;
		
		float dx1	= d1.x * w * 0.5;		// x component of direction 1, multiplied by width.
		float dy1	= d1.y * w * 0.5;		// y component of direction 1, multiplied by width.
		float dx2	= d2.x * w * 0.5;		// x component of direction 2, multiplied by width.
		float dy2	= d2.y * w * 0.5;		// y component of direction 2, multiplied by width.
		
		float v1x = p1.x - dx1;		// vertex 1, x component.
		float v1y = p1.y - dy1;		// vertex 1, y component.
		
		float v2x = p1.x + dx1;		// vertex 2, x component.
		float v2y = p1.y + dy1;		// vertex 2, y component.
		
		float v3x = p2.x + dx2;		// vertex 3, x component.
		float v3y = p2.y + dy2;		// vertex 3, x component.
		
		float v4x = p2.x - dx2;		// vertex 4, x component.
		float v4y = p2.y - dy2;		// vertex 4, x component.
		
		ofBeginShape();
		ofVertex( v1x, v1y );
		ofVertex( v2x, v2y );
		ofVertex( v3x, v3y );
		ofVertex( v4x, v4y );
		ofEndShape( true );
	}
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

