/*
 *  NawlzFishBowl.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#include "MSACore.h"
#include "MSAFluid.h"

#include "ofBaseApp.h"
#include "ofTexture.h"

#include "NawlzParticle.h"
#include "NawlzParticle01.h"
#include "NawlzFishBowlParticle.h"

using namespace MSA;

//////////////////////////////////////////////
//	.
//////////////////////////////////////////////

class NawlzFishBowl : public ofBaseApp
{
	
public:
	
	 NawlzFishBowl();
	~NawlzFishBowl();
	
	void setup	();
	void update	();
	void draw	();
	
	void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	void createParticleTexture		( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	void createBowlTexture			( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	void initFluid	();
	void createParticles			();
	
	void updateParticles			();
	bool isPointInsideROI			( float x, float y );
	bool isPointInsideBowl			( float x, float y );
	
	void drawParticles	();
	
	void addToFluid		( Vec2f pos, Vec2f vel, bool addColor, bool addForce );
	
	bool		bDrawBackground;
	bool		bDrawParticles;
	bool		bDrawBowl;
	bool		bOverRoi;
	bool		bTouchDown;

	vector<NawlzFishBowlParticle*>	particles;
	
	ofRectangle		roi;
	
	ofTexture*		backgroundTexture;
	ofTexture*		particleTexture;
	ofTexture*		bowlTexture;
	unsigned char*	bowlPixels;
	ofRectangle		bowlRect;
	int				bowlPixelDepth;
	
	ofPoint			backgroundTextureXY;
	ofPoint			bowlTextureXY;
	
	FluidSolver		fluidSolver;
	int				fluidCellsX;
	float			fluidScale;
	bool			bResizeFluid;
	Vec2f			pMouse;
	
	bool			fluidEnableRGB;
	float			fluidFadeSpeed;
	float			fluidDeltaT;
	float			fluidVisc;
	float			fluidColorDiffusion;
	int				fluidSolverIterations;
	bool			fluidEnableVorticityConfinement;
	bool			fluidWrapX;
	bool			fluidWrapY;
	bool			fluidInputVelocityMult;
	
};