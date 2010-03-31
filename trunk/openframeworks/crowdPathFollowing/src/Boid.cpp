/*
 *  Boid.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 28/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 *	PORTED FROM, http://www.shiffman.net/teaching/nature/path-following/
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
	predict.set( 0, 0 );		// reset.
	predict.set( vel );
	predict.normalize();
	predict *= 25;

	predictLoc.set( 0, 0 );		// reset.
	predictLoc = loc + predict;
	
	target.set( 0, 0 );			// reset.
	
    float record = 1000000;
	
	for( int i=0; i<path.points.size(); i++ )
	{
		ofxVec2f a;
		a.set( path.points.at( i ) );									// current point.
		
		ofxVec2f b;
		b.set( path.points.at( ( i + 1 ) % path.points.size() ) );		// next point.
		
		// the normal is a vector that extends from that point and is perpendicular to the line.
		// http://www.shiffman.net/itp/classes/nature/pathimages/path5normal.jpg
		// http://www.shiffman.net/itp/classes/nature/pathimages/path6normal.jpg
		
		ofxVec2f normal;
		normal = getNormalPoint( predictLoc, a, b );
		
		// check if normal is on line segment
		
		float da;
		da = normal.distance( a );
		
		float db;
		db = normal.distance( b );
		
		ofxVec2f line;
		line = b - a;
		
		// if it's not within the line segment, consider the normal to just be the end of the line segment (point b)

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

void Boid :: seek( const ofxVec2f& trg )
{
	acc += steer( trg, false );
}

void Boid :: arrive( const ofxVec2f& trg )
{
	acc += steer( trg, true );
}

ofxVec2f Boid :: steer( const ofxVec2f& trg, bool slowdown )
{
	ofxVec2f steer;
    
	ofxVec2f desired;
	desired = trg - loc;
    
	float d;
	d = desired.length();
	
	if( d > 0 )
	{
		desired.normalize();
		if( slowdown && ( d < 100.0f ) )
		{	
			desired += maxspeed * ( d / 100.0f );
		}
		else
		{
			desired *= maxspeed;
		}
		
		steer = desired - vel;		// Steering = Desired minus Velocity
		steer.limit( maxforce );	// Limit to maximum steering force
    } 
    else
	{
		steer.set( 0, 0 );
    }
	
    return steer;
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

void Boid :: render()
{
	ofSetColor( 75, 75, 75 );
	ofFill();
	glPushMatrix();
	glTranslatef( loc.x, loc.y, 0 );
	ofEllipse( 0, 0, r, r );
	glPopMatrix();
}

void Boid :: renderDebug ()
{
	// red line from boid to normal point.
	
	ofSetColor( 255, 0, 0 );
	ofLine( loc.x, loc.y, predictLoc.x, predictLoc.y );

	// red dot is the normal point.
	
	ofFill();
	ofEllipse( predictLoc.x, predictLoc.y, 4, 4 );
	
	// blue line, perpendicular from normal point to line.
	
	ofSetColor( 0, 0, 255 );
	ofLine( predictLoc.x, predictLoc.y, target.x, target.y );
	
	// blue point, on line.
	
	ofFill();
	ofEllipse( target.x, target.y, 4, 4 );
	
	// green circle, direction from target on line.
	
	ofSetColor( 0, 255, 0 );
	ofNoFill();
	ofEllipse( target.x + dir.x, target.y + dir.y, 8, 8 );
}
