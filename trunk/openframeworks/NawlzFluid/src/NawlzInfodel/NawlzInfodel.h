//
//  NawlzJackInBox.h
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "MSACore.h"
#include "MSAFluid.h"

#include "NawlzInfodelParticle.h"

using namespace MSA;

struct ParticleConnection
{
    float p1x, p1y;
    float p2x, p2y;
    float p1s, p2s;
};

class NawlzInfodel : public ofBaseApp
{
    
public:
    
     NawlzInfodel   ();
    ~NawlzInfodel   ();
    
    void setup  ();
    void update ();
    void draw   ();
    
    void killParticles              ( vector<NawlzInfodelParticle*>& particles );
    
    void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createHarleyOneTexture     ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createHarleyTwoTexture     ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createParticleTexture      ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    
    void toggleHarley               ();
    void showHarleyOne              ();
    void showHarleyTwo              ();
    
    void updateParticles            ( vector<NawlzInfodelParticle*>& particles, vector<ParticleConnection>& connections );
    void addParticle                ( vector<NawlzInfodelParticle*>& particles );
    void addToFluid                 ( Vec2f pos, Vec2f vel, bool addColor, bool addForce );
    
    void updateFluid                ();
    
    void drawParticles              ( vector<NawlzInfodelParticle*>& particles  );
    void drawConnections            ( vector<ParticleConnection>& connections );
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    ofTexture*      backgroundTexture;
    ofTexture*      particleTexture;
    ofTexture*      harleyOneTexture;
    ofTexture*      harleyTwoTexture;
    
    ofRectangle     bounds;
    ofRectangle     harleyOneRect;
    ofRectangle     harleyTwoRect;
    int             harleyMode;
    
    vector<NawlzInfodelParticle*>   particles0;
    vector<NawlzInfodelParticle*>   particles1;
    vector<ParticleConnection>      connections0;
    vector<ParticleConnection>      connections1;
    int                             connectionLength;
    float                           connectionAlpha;
    
    //------ fluid.
    
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