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
#include "BoidObstacle.h"
#include "ofxCvBlob.h"

class Flock
{

public :
	
	 Flock();
	~Flock();
	
	void init			();
	void addBoids		( int num );
	void addBoid		( Boid &boid );
	void addQueen		();
	
	void setContainer	( ofRectangle &rect );
	void setBoidFrames	( vector<ofImage> *boidFramesPtr );
	
	void update				();
	void updateBoidPartOne	( Boid &boid, bool bUpdateVars = true );
	void updateBoidPartTwo	( Boid &boid );
	void updateForces		();
	void updateFood			();
	
	void draw			();
	void drawBoids		();
	void drawMice		();
	void drawFood		();
	void drawObstacles	();
	void drawHome		();

	void addFood		( int x, int y );
	void addObstacle	( int x, int y );
	void addMouse		( int x, int y );
	void addBlobs		( vector<ofxCvBlob> *blobs );
	
	int					boidsNum;
	int					boidsNumRevised;
	
	ofRectangle			containerRect;
	
	vector<Boid>		boids;
	vector<BoidForce>	forces;
	Boid				queen;
	
	vector<ofImage>		*boidFrames;
	
	vector<BoidFood>		foods;
	vector<BoidObstacle>	obstacles;
	vector<BoidForce>		mice;
	BoidForce				mouseCopy;
	BoidForce				home;
	
	vector<ofxCvBlob>		*blobs;
	
	float				boidSeperationWeight;
	float				boidAlignmentWeight;
	float				boidCohesionWeight;
	float				boidRandomWeight;
	
	float				boidSeparationDist;
	float				boidPerception;
	float				boidMaxSpeed;
	float				boidMaxForce;
	
	bool				bMouseMoved;
	float				mouseReach;
	float				mouseForce;
	
	float				boidsInTree;
};

#endif