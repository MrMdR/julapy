/*
 *  Boid.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 28/03/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BOID
#define BOID

#include "ofMain.h"
#include "Path.h"
#include "ofxVectorMath.h"

class Boid
{

public :
	
	void init( const ofxVec2f& l, float ms, float mf );
	void applyForces( const vector<Boid>& boids, const Path& path );
	void run();
	ofxVec2f follow( const Path& path );
	ofxVec2f getNormalPoint( const ofxVec2f& p, const ofxVec2f& a, const ofxVec2f& b );
	ofxVec2f separate( const vector<Boid>& boids );
	void update();
	void seek( const ofxVec2f& target );
	void arrive( const ofxVec2f& target );
	ofxVec2f steer( const ofxVec2f& target, bool slowdown );
	void render();
	void borders();
	
	ofxVec2f	loc;
	ofxVec2f	vel;
	ofxVec2f	acc;
	float		r;
	float		maxforce;    // Maximum steering force
	float		maxspeed;    // Maximum speed
	
};

#endif