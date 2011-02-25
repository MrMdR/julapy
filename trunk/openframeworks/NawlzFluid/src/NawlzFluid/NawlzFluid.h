/*
 *  NawlzFluid.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MSACore.h"
#include "MSAFluid.h"

#include "ofBaseApp.h"
#include "ofTexture.h"

#include "NawlzParticle.h"

using namespace MSA;

//////////////////////////////////////////////
//	MESH STRUCT
//////////////////////////////////////////////

struct mesh_point
{
	Vec2f p;
	Vec2f v;
	Vec2f f;
};

struct mesh_quad
{
	float x;
	float y;
	float w;
	float h;
	float tx;
	float ty;
	
	int mesh_point_index[ 4 ];
	
	GLfloat ver_coords[ 8 ];
	GLfloat tex_coords[ 8 ];
	
	bool bTextureInside;
};

//////////////////////////////////////////////
//	MOTION.
//////////////////////////////////////////////

class CircleMotion
{

public:
	
	CircleMotion()
	{
		radius		= 200;
		angle		= 0;
		angleInc	= PI * 0.04;
		centerX		= 500;
		centerY		= 300;
		
		update();
	};
	
	~CircleMotion () {};
	
	float radius;
	float angle;
	float angleInc;
	float centerX;
	float centerY;
	float x;
	float y;
	
	void update ()
	{
		x = centerX + radius * cos( angle );
		y = centerY + radius * sin( angle );
		
		angle += angleInc; 
	}
	
};

//////////////////////////////////////////////
//	FLUID
//////////////////////////////////////////////

class NawlzFluid : public ofBaseApp
{

public:
	
	 NawlzFluid();
	~NawlzFluid();
	
	void setup	();
	void update	();
	void draw	();

	void createImageTexture			( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
	void createParticleTexture		( unsigned char* pixels, int width, int height, int glType );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	bool	bDrawMeshGrid;
	bool	bDrawMeshQuads;
	bool	bDrawParticles;
	bool	bDrawBackground;
	bool	bDrawVectors;
	bool	bDrawFluid;
	bool	useCircleMotion;
	
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
	
	int		particlesNumPerShoot;
	
private:
	
	void initMesh	();
	void initFluid	();
	
	void createFluidTexture			();
	
	void updateMeshPointsWithMouse	();
	void updateMeshPointsWithFluid	();
	void updateMeshQuads			();
	void updateParticles			();
	
	void drawMeshQuads	();
	void drawMeshPoints	();
	void drawMeshGrid	();
	void drawMeshLines	();
	void drawParticles	();
	
	void addToFluid		( Vec2f pos, Vec2f vel, bool addColor, bool addForce );
	void drawVectors	( float x, float y, float renderWidth, float renderHeight );
	void drawFluid		();
	
	int			mouseX;
	int			mouseY;
	
	ofTexture*		backgroundTexture;
	ofTexture*		whaleTexture;
	ofTexture*		particleTexture;
	ofTexture*		fluidTexture;
	unsigned char*	fluidPixels;
	
	ofPoint			backgroundTextureXY;
	ofPoint			whaleTextureXY;
	
	int					mesh_w_inc;
	int					mesh_h_inc;
	vector<mesh_quad>	mesh_quads;
	vector<mesh_point>	mesh_points;
	
	FluidSolver			fluidSolver;
	int					fluidCellsX;
	bool				bResizeFluid;
	Vec2f				pMouse;
	
	vector<NawlzParticle>	particles;
	
	CircleMotion		circleMotion;
};