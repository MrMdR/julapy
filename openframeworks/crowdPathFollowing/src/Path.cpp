/*
 *  Path.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 28/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 *	PORTED FROM, http://www.shiffman.net/teaching/nature/path-following/
 */

#include "Path.h"

void Path :: init ()
{
	radius = 20;
}

void Path :: addPoint( float x, float y )
{
	points.push_back( ofxVec2f() );
	points.back().set( x, y );
}

void Path :: display ()
{
	for( int i=0; i<points.size(); i++ )
	{
		ofxVec2f& point = points.at( i );
		
		ofSetColor( 175, 175, 175 );
		ofNoFill();
		ofEllipse( point.x, point.y, radius * 2, radius * 2 );
    }
	
    for( int i=0; i<points.size(); i++ )
	{
		ofxVec2f& start = points.at( i );
		ofxVec2f& end	= points.at( ( i + 1 ) % points.size() );
		ofxVec2f line	= end - start;
		
		ofxVec2f normal;
		normal.set( line.y, -line.x );
		normal.normalize();
		normal *= radius;
		
		ofxVec2f a = start + normal;
		ofxVec2f b = end + normal;
		ofxVec2f c = end - normal;
		ofxVec2f d = start - normal;
		
		ofSetColor( 175, 175, 175 );
		ofFill();
		ofBeginShape();
		ofVertex( a.x, a.y );
		ofVertex( b.x, b.y );
		ofVertex( c.x, c.y );
		ofVertex( d.x, d.y );
		ofEndShape( false );
    }

	ofSetColor( 0, 0, 0 );
	ofNoFill();
	ofBeginShape();
    for( int i=0; i<points.size(); i++ )
	{
		ofxVec2f& loc = points.at( i );
		ofVertex( loc.x, loc.y );
    }
    ofEndShape( true );
}