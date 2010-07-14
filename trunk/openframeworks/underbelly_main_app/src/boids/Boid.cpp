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
	home			= NULL;
	
	bPullHome		= true;
	bIsHome			= false;
	bLeftHome		= false;
	homeInTime		= 0;
	homeInTimeout	= 0;
	homeOutTime		= 0;
	homeOutTimeout	= 0;
	
	frames			= NULL;
	frameIndex		= 0;
	framesTotal		= 0;
	frameW			= 0;
	frameH			= 0;
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

void Boid :: setHome ( BoidForce *homePtr )
{
	home = homePtr;
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

void Boid :: setFrames ( vector<ofImage> *framesPtr )
{
	frames		= framesPtr;
	framesTotal	= frames->size();
	frameIndex	= (int)( ofRandom( 0, framesTotal - 1 ) );
	frameW		= frames->at( 0 ).width;
	frameH		= frames->at( 0 ).height;
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void Boid :: update_acc ()
{
	accNew = 0;									// zero the acceleration.
	
	//-- point forces.
	
	ofxVec2f ptf;						

	if( bPullHome && home != NULL )
	{
		int homeX = home->x * containerRect.width;
		int homeY = home->y * containerRect.height;
		
		ptf += pointForce( homeX, homeY, home->reach, home->magnitude );
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
		int t;
		t = trailPos.size();
		
		dir  = trailPos[ t - 1 ];
		dir -= trailPos[ t - 2 ];
		dir.normalize();
		
		perp.set( dir );
		perp.perpendicular();
		
		trailDir.push_back( perp );
		
		//-- random trail displacement.
		
		bool bAddRndDisplace;
		bAddRndDisplace = true;
		
		if( bAddRndDisplace )
		{
			trailRnd.push_back( ofPoint() );
			ofPoint &rnd = trailRnd.back();
			float rd	= 10;
			float r		= ofRandom( -rd, rd );
			rnd.x		= perp.x * r;
			rnd.y		= perp.y * r;
		}
		
		//-- if max numbe is reached, start removing old trail positions.
		
		if( trailPos.size() > trailLength )				
		{
			int diff;
			diff = trailPos.size() - trailLength;
			
			trailPos.erase( trailPos.begin(), trailPos.begin() + diff );
			trailDir.erase( trailDir.begin(), trailDir.begin() + diff );
			trailRnd.erase( trailRnd.begin(), trailRnd.begin() + diff );
		}
		
		//-- work out vertices & color.
		
		trailVrt.clear();
		trailCol.clear();
		
		t = trailPos.size();
		
		for( int i=0; i<t; i++ )
		{
			ofxVec2f &p1 = trailPos[ i ];
			ofxVec2f &d1 = trailDir[ i ];
			ofPoint  &r1 = trailRnd[ i ];
			
			float p		= i / (float)( t - 1 );
			float s		= cos( PI * 0.5 * p );
			float si	= 1 - s;
			float w		= 10 * si;
			float a		= si;
			
			float dx1	= d1.x * w * 0.5;		// x component of direction 1, multiplied by width.
			float dy1	= d1.y * w * 0.5;		// y component of direction 1, multiplied by width.
			
			float v1x	= p1.x - dx1;			// vertex 1, x component.
			float v1y	= p1.y - dy1;			// vertex 1, y component.
			
			float v2x	= p1.x + dx1;			// vertex 2, x component.
			float v2y	= p1.y + dy1;			// vertex 2, y component.
			
			if( bAddRndDisplace )
			{
				v1x += r1.x * s;
				v1y += r1.y * s;
				v2x += r1.x * s;
				v2y += r1.y * s;
			}

			trailVrt.push_back( ofPoint() );	// vertex 1
			ofPoint &vrt_1 = trailVrt.back();
			vrt_1.set( v1x, v1y, 0 );
			
			trailVrt.push_back( ofPoint() );	// vertex 1
			ofPoint &vrt_2 = trailVrt.back();
			vrt_2.set( v2x, v2y, 0 );
			
			trailCol.push_back( ofColor() );	// color of vertex 1.
			trailCol.back().r = 255;
			trailCol.back().g = 255;
			trailCol.back().b = 255;
			trailCol.back().a = 255 * a;
			
			trailCol.push_back( ofColor() );	// color of vertex 1.
			trailCol.back().r = 255;
			trailCol.back().g = 255;
			trailCol.back().b = 255;
			trailCol.back().a = 255 * a;
		}
	}
	else	// this is the first trail position, nothing to work out.
	{
		trailDir.push_back( ofxVec2f() );
		trailRnd.push_back( ofPoint() );
	}
}

void Boid :: update_home ()
{
	if( home == NULL )
		return;
	
	int msElapsed;
	msElapsed = ofGetElapsedTimeMillis();
	
	if( bPullHome && !bIsHome )
	{
		float homeX = home->x * containerRect.width;
		float homeY = home->y * containerRect.height;
		float dist	= ofDist( pos.x, pos.y, homeX, homeY );
		
		if( dist < home->size )
		{
			bIsHome			= true;
			
			homeInTime		= msElapsed;
			homeInTimeout	= (int)( ofRandom( 5000, 20000 ) );
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
			homeOutTimeout	= (int)( ofRandom( 2000, 4000 ) );
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

void Boid :: update_frame ()
{
	if( frames == NULL )
		return;
	
	if( ++frameIndex > framesTotal - 1 )
		frameIndex = 0;
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void Boid :: drawHead ()
{
	if( frames == NULL )
	{
		ofFill();
		ofCircle( pos.x, pos.y, size );
	}
	else 
	{
		float s;
		s = 0.5;
		
		float x;
		x = pos.x - frameW * s * 0.5;
		
		float y;
		y = pos.y - frameH * s * 0.5;
		
		glPushMatrix();
		glTranslatef( x, y, 0 );
		glScalef( s, s, 0 );
		
		ofImage &img = frames->at( frameIndex );
		img.draw( 0, 0 );
		
		glPopMatrix();
	}
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
	ofStyle style = ofGetStyle();
	if( style.smoothing && !style.bFill )
	{
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glEnable( GL_LINE_SMOOTH );
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	glBegin( GL_QUAD_STRIP );
//	glBegin( GL_LINE_LOOP );
//	glBegin( GL_LINE_STRIP );
//	glBegin( GL_LINES );
	
	for( int i=0; i<trailVrt.size(); i+=2 )
	{
		int j = i + 0;
		int k = i + 1;
		
		ofPoint &v1 = trailVrt[ j ];
		ofPoint &v2 = trailVrt[ k ];
		
		ofColor &c1 = trailCol[ j ];
		ofColor &c2 = trailCol[ k ];
		
		ofSetColor( c1.r, c1.g, c1.b, c1.a );
		glVertex2f( v1.x, v1.y );

		ofSetColor( c2.r, c2.g, c2.b, c2.a );
		glVertex2f( v2.x, v2.y );
	}
	
	glEnd();
}

void Boid :: drawTrailLine ()
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

