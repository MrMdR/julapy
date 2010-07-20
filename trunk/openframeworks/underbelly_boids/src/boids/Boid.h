/*
 *  Boid.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 5/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BOID_H
#define BOID_H

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "BoidForce.h"
#include "BoidFood.h"

class Boid 
{

public :
	
	 Boid();
	~Boid();
	
	void setBoids		( vector<Boid> *boidsPtr );
	void setForces		( vector<BoidForce> *forcesPtr );
	void setFoods		( vector<BoidFood> *foodsPtr );
	void setPosition	( float x, float y );
	void setVelocity	( float x, float y );
	void setContainer	( ofRectangle &rect );
	
	void update_acc		();
	void update_vel		();
	void update_pos		();
	void update_final	();
	
	void draw			();
	void drawDebug		();
	void drawTrail		();
	void drawTrailFill	();
	
	ofxVec2f separate		( vector<Boid> *boids );
	ofxVec2f align			( vector<Boid> *boids );
	ofxVec2f cohesion		( vector<Boid> *boids );
	ofxVec2f reynoldsLimit	( ofxVec2f &desired );
	ofxVec2f pointForce		( float x, float y, float reach, float magnitude );

	vector<Boid>		*boids;
	vector<BoidFood>	*foods;
	vector<BoidForce>	*forces;

	ofxVec2f			pos;
	ofxVec2f			vel;
	ofxVec2f			acc;
	
	ofxVec2f			posNew;
	ofxVec2f			velNew;
	ofxVec2f			accNew;
	
	ofxVec2f			dir;
	ofxVec2f			perp;
	
	vector<ofxVec2f>	trailPos;
	vector<ofxVec2f>	trailDir;
	vector<int>			trailCol;
	int					trailLength;
	
	float				size;
	float				separationDist;
	float				perception;
	float				maxSpeed;
	float				maxForce;
	
	float				separationWeight;
	float				alignmentWeight;
	float				cohesionWeight;
	
	bool				bContain;
	ofRectangle			containerRect;
};

#endif