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
#include "BoidForce.h"
#include "BoidFood.h"

class Flock
{

public :
	
	 Flock();
	~Flock();
	
	void init			();
	void addBoids		( int num );
	void addBoid		( Boid &boid );
	
	void update			();
	void updateForces	();
	
	void draw			();
	void drawBoids		();
	void drawMice		();
	void drawFood		();

	void addFood		( int x, int y );
	void addMouse		( int x, int y );
	
	int					boidsNum;
	int					boidsNumRevised;
	
	vector<Boid>		boids;
	vector<BoidForce>	forces;
	
	vector<BoidFood>	foods;
	vector<BoidForce>	obstacles;
	vector<BoidForce>	mice;
	BoidForce			mouseCopy;
	
	float				boidSeperationWeight;
	float				boidAlignmentWeight;
	float				boidCohesionWeight;
	
	float				boidSeparationDist;
	float				boidPerception;
	float				boidMaxSpeed;
	float				boidMaxForce;
	
	bool				bMouseMoved;
	float				mouseReach;
	float				mouseForce;
};

#endif