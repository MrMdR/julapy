//
//  NawlzJackInBox.h
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "NawlzJackInBoxPart.h"

#include "MSACore.h"
#include "MSAFluid.h"

using namespace MSA;

class NawlzJackInBox : public ofBaseApp
{
    
public:
    
     NawlzJackInBox ();
    ~NawlzJackInBox ();
    
    void setup  ();
    void update ();
    void draw   ();
    
    void initFluid                  ();
    void addToFluid                 ( Vec2f pos, Vec2f vel, bool addColor, bool addForce );
    void drawTexRotated             ( ofTexture* tex, const ofPoint& pos, const ofPoint& reg, float angle );
    
    void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createTongueTexture        ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    ofTexture*  backgroundTexture;
    
    ofTexture*  tongueTex;
    ofRectangle tongueTexRect;
    ofPoint     tongueTexRegPoint;
    
    vector<NawlzJackInBoxPart*> parts;
    
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