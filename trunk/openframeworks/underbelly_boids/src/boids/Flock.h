/*
 *  Flock.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 8/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FLOCK_H
#define FLOCK_H

#include "ofMain.h"
#include "Boid.h"
#include "BoidFood.h"

class Flock
{

public :
	
	 Flock();
	~Flock();
	
	void init			();
	
	void update			();
	void updateForces	();
	
	void draw			();
	void drawBoids		();
	void drawForces		();
	void drawFood		();

	void addForce		( float x, float y, float reach, float magnitude );
	void clearForces	();
	
	void addFood		( int x, int y );
	void addMouse		( int x, int y );
	
	int					boidsNum;
	vector<Boid>		boids;
	vector<BoidForce>	forces;
	vector<BoidFood>	foods;
	
	float				boidSeperationWeight;
	float				boidAlignmentWeight;
	float				boidCohesionWeight;
	
	float				boidSeparationDist;
	float				boidPerception;
	float				boidMaxSpeed;
	float				boidMaxForce;
	
	ofPoint				mousePoint;
	bool				bMouseMoved;
	float				mouseReach;
	float				mouseForce;
	
};

#endif