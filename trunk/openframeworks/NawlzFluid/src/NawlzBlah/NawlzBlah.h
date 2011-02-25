/*
 *  NawlzBlah.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofBaseApp.h"
#include "ofTexture.h"

#include "MSACore.h"
#include "MSAFluid.h"

#include "NawlzBlahParticle.h"

using namespace MSA;

class NawlzBlah : public ofBaseApp
{
	
public:
	
	 NawlzBlah();
	~NawlzBlah();
	
	void setup	();
	void update	();
	void draw	();
	
	void initFluid				();
	
	void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	void createParticleTexture		( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	
	void addParticle	();
	void addToFluid		( Vec2f pos, Vec2f vel, bool addColor, bool addForce );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	ofTexture*		backgroundTexture;
	ofTexture*		particleTexture;
	
	vector<NawlzBlahParticle*> particles;
	
	float wanderRadius;
	float wanderDistance;
	float wanderChange;
	float wanderEase;
	
	//---- fluid.
	
	FluidSolver		fluidSolver;
	int				fluidCellsX;
	bool			bResizeFluid;
	Vec2f			pMouse;
	
	bool	fluidEnableRGB;
	float	fluidFadeSpeed;
	float	fluidDeltaT;
	float	fluidVisc;
	float	fluidColorDiffusion;
	int		fluidSolverIterations;
	bool	fluidEnableVorticityConfinement;
	bool	fluidWrapX;
	bool	fluidWrapY;
	bool	fluidInputVelocityMult;
	float	fluidScale;
};