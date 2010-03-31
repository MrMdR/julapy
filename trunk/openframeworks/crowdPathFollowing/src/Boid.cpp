/*
 *  Boid.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 28/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Boid.h"

void Boid :: init( const ofxVec2f& l, float ms, float mf )
{
	loc			= l;
	r			= 12;
	maxspeed	= ms;
	maxforce	= mf;
	vel.set( maxspeed, 0 );
}

void Boid :: applyForces( const vector<Boid>& boids, const Path& path )
{
	ofxVec2f f = follow( path );
	ofxVec2f s = separate( boids );
	f	*= 3;
	s	*= 1;
	acc += f;
	acc += s;
}

void Boid :: run ()
{
	update();
	borders();
}

ofxVec2f Boid :: follow( const Path& path )
{
	ofxVec2f predict;
	predict.set( vel );
	predict.normalize();
	predict *= 25;
	
	ofxVec2f predictLoc;
	predictLoc = loc + predict;
	
	bool debug = true;
	
	if (debug)
	{
		ofSetColor( 0, 0, 0 );
		ofFill();
		ofLine( loc.x, loc.y, predictLoc.x, predictLoc.y );
		ofEllipse( predictLoc.x, predictLoc.y, 4, 4 );
	}
	
    ofxVec2f target;
    ofxVec2f dir;
    float record = 1000000;
	
    for( int i = 0; i < path.points.size(); i++ )
	{
		ofxVec2f a;
		a.set( path.points.at( i ) );
		
		ofxVec2f b;
		b.set( path.points.at( ( i + 1 ) % path.points.size() ) );
		
		ofxVec2f normal;
		normal = getNormalPoint( predictLoc, a, b );
		
		float da;
		da = normal.distance( a );
		
		float db;
		db = normal.distance( b );
		
		ofxVec2f line;
		line = b - a;

		if( da + db > line.length() + 1 )
		{
			normal.set( b );
			
			a.set( path.points.at( ( i + 1 ) % path.points.size() ) );
			b.set( path.points.at( ( i + 2 ) % path.points.size() ) );
			line = b - a;
		}
		
		float d;
		d = predictLoc.distance( normal );
		
		if( d < record )
		{
			record = d;
			target.set( normal );
			
			dir.set( line );
			dir.normalize();
			dir *= 25;
		}
    }
	
    if( debug )
	{
		ofSetColor( 0, 0, 0 );
		ofFill();
		ofLine( predictLoc.x, predictLoc.y, target.x, target.y );
		ofEllipse( target.x, target.y, 4, 4 );
		ofNoFill();
		ofLine( predictLoc.x, predictLoc.y, target.x, target.y );
		if( record > path.radius )
		{
			ofSetColor( 255, 0, 0 );
			ofFill();
		}
		ofNoFill();
		ofEllipse( target.x + dir.x, target.y + dir.y, 8, 8 );
    }
	
    if( record > path.radius || vel.length() < 0.1 )
	{
		target += dir;
		return steer( target, false );
    } 
    else
	{
		ofxVec2f v;
		return v;
    }
}


ofxVec2f Boid :: getNormalPoint( const ofxVec2f& p, const ofxVec2f& a, const ofxVec2f& b )
{
	ofxVec2f ap;
	ap = p - a;
    
	ofxVec2f ab;
	ab = b - a;
    ab.normalize();
	
	ab *= ap.dot( ab );
	
    ofxVec2f normalPoint;
	normalPoint = a + ab;
    
	return normalPoint;
}

ofxVec2f Boid :: separate( const vector<Boid>& boids )
{
	float desiredseparation = r * 2;
	ofxVec2f steer;
	int count = 0;
	
	for( int i=0; i<boids.size(); i++ )
	{
		const Boid& other = boids.at( i );
		
		float d;
		d = loc.distance( other.loc );
		
		if( ( d > 0 ) && ( d < desiredseparation ) )
		{
			ofxVec2f diff;
			diff = loc - other.loc;
			diff.normalize();
			diff /= d;
			steer += diff;
			count++;
		}
	}

    if( count > 0 )
	{
		steer /= (float)count;
	}
	
    if( steer.length() > 0 )
	{
		steer.normalize();
		steer *= maxspeed;
		steer -= vel;
		steer.limit( maxforce );
    }
    return steer;
}

void Boid :: update()
{
	vel += acc;
	vel.limit( maxspeed );
	
	loc += vel;
	
	acc *= 0;	// reset.
}

void Boid :: seek( const ofxVec2f& target )
{
	acc += steer( target, false );
}

void Boid :: arrive( const ofxVec2f& target )
{
	acc += steer( target, true );
}

ofxVec2f Boid :: steer( const ofxVec2f& target, bool slowdown )
{
	ofxVec2f steer;
    
	ofxVec2f desired;
	desired = target - loc;
    
	float d;
	d = desired.length();
	
	if( d > 0 )
	{
		desired.normalize();
		if( ( slowdown ) && ( d < 100.0f ) )
		{	
			desired += maxspeed * ( d / 100.0f );
		}
		else
		{
			desired *= maxspeed;
		}
		steer = desired - vel;
		steer.limit(maxforce);  // Limit to maximum steering force
    } 
    else
	{
		steer.set( 0, 0 );
    }
	
    return steer;
}

void Boid :: render()
{
	ofSetColor( 75, 75, 75 );
	ofFill();
	glPushMatrix();
	glTranslatef( loc.x, loc.y, 0 );
	ofEllipse( 0, 0, r, r );
	glPopMatrix();
}

void Boid :: borders()
{
	if( loc.x < -r )
	{
		loc.x = ofGetWidth() + r;
	}
	
    if( loc.x > ofGetWidth() + r )
	{
		loc.x = -r;
	}
}
