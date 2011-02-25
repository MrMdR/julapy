/*
 *  NawlzBlah.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzBlah.h"

NawlzBlah :: NawlzBlah()
{
	wanderChange	= 0.12;
	wanderRadius	= 16.0;
	wanderDistance	= 60.0;
	wanderEase		= 0.2;
}

NawlzBlah :: ~NawlzBlah()
{
	if( backgroundTexture )
	{
		backgroundTexture->clear();
		delete backgroundTexture;
		backgroundTexture = NULL;
	}
	
	if( particleTexture )
	{
		particleTexture->clear();
		delete particleTexture;
		particleTexture = NULL;
	}
	
	int t = particles.size();			// delete all particles.
	for( int i=0; i<t; i++ )
	{
		NawlzBlahParticle* particle;
		particle = particles[ i ];
		
		particles.erase( particles.begin() + i );
		
		--i;
		--t;
		
		delete particle;
		particle = NULL;
	}		
}

///////////////////////////////////////////
//	TEXTURES.
///////////////////////////////////////////

void NawlzBlah :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzBlah :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void NawlzBlah :: setup	()
{
	initFluid();
}

void NawlzBlah :: initFluid ()
{
	fluidEnableRGB					= false;
	fluidFadeSpeed					= 0.002;
	fluidDeltaT						= 0.15;
	fluidVisc						= 0.001;
	fluidColorDiffusion				= 0;
	fluidSolverIterations			= 5;
	fluidEnableVorticityConfinement	= false;
	fluidWrapX						= false;
	fluidWrapY						= false;
	fluidInputVelocityMult			= 0.2;
	fluidScale						= 0;
	
	fluidSolver.setup( 100, 100 );
	fluidSolver.enableRGB( fluidEnableRGB );
	fluidSolver.setFadeSpeed( fluidFadeSpeed );
	fluidSolver.setDeltaT( fluidDeltaT );
	fluidSolver.setVisc( fluidVisc );
	fluidSolver.setColorDiffusion( fluidColorDiffusion );
	fluidSolver.setSolverIterations( fluidSolverIterations );
	fluidSolver.enableVorticityConfinement( fluidEnableVorticityConfinement );
	fluidSolver.setWrap( fluidWrapX, fluidWrapY );
	
	fluidCellsX		= 50;
	bResizeFluid	= true;
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzBlah :: update ()
{
	if( bResizeFluid )
	{
		float r;
		r = ofGetWidth() / (float)ofGetHeight();
		
		fluidSolver.setSize( fluidCellsX, fluidCellsX / r );
		
		bResizeFluid = false;
	}
	
	fluidSolver.enableRGB( fluidEnableRGB );
	fluidSolver.setFadeSpeed( fluidFadeSpeed );
	fluidSolver.setDeltaT( fluidDeltaT );
	fluidSolver.setVisc( fluidVisc );
	fluidSolver.setColorDiffusion( fluidColorDiffusion );
	fluidSolver.setSolverIterations( fluidSolverIterations );
	fluidSolver.enableVorticityConfinement( fluidEnableVorticityConfinement );
	fluidSolver.setWrap( fluidWrapX, fluidWrapY );
	fluidSolver.update();
	
	fluidScale *= 0.98;		// decay fluid scale.
	
	//--- particles.
	
	Vec2f fluidVel;
	
	float maxVel;
	maxVel = 0.00025;
	maxVel = 0.0005;
	maxVel = 0.001;
	maxVel = 0.002;
	
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzBlahParticle* particle;
		particle = particles[ i ];
		
		particle->wanderChange		= wanderChange;
		particle->wanderRadius		= wanderRadius;
		particle->wanderDistance	= wanderDistance;
		particle->wanderEase		= wanderEase;
		
		particle->update( 1 - fluidScale );
		
		//--- calc fluid force.
		
		float px = particle->loc.x / (float)ofGetWidth();
		float py = particle->loc.y / (float)ofGetHeight();
		float lp = particle->lifePercent;
		
		Vec2f p( px, py );
		fluidVel = fluidSolver.getVelocityAtPos( p );
		
		fluidVel.limit( maxVel );
		fluidVel *= 10000;
		fluidVel *= fluidScale * fluidScale;
		fluidVel *= particle->friction;
		fluidVel *= ( lp < 0.1 ) ? 0 : 1.0;
		fluidVel *= ( lp < 0.15 ) ? ( ( lp - 0.1 ) / 0.15 ) : 1.0;
		
		particle->loc.x += fluidVel.x;
		particle->loc.y += fluidVel.y;
		
		//--- remove dead particles.
		
		if( !particle->isAlive() )
		{
			particles.erase( particles.begin() + i );
			
			--i;
			--t;
			
			delete particle;
			particle = NULL;
		}
	}
	
	if( ofRandom( 0, 1.0 ) > 0.8 )
	{
		addParticle();
	}
}

void NawlzBlah :: addParticle ()
{
	Vec2f vel;
	vel.set( 1, 0 );
	vel.rotate( ofRandom( -20, 20 ) * DEG_TO_RAD );
	vel *= ofRandom( 1.0, 3.0 );
	
	Vec2f pos;
	pos.x = 539;
	pos.y = 346;
	
	NawlzBlahParticle* particle;
	particle = new NawlzBlahParticle();
	particle->setLoc( pos.x, pos.y );
	particle->setVel( vel.x, vel.y );
	particle->setTexture( particleTexture );
	
	particles.push_back( particle );
}

void NawlzBlah :: addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce )
{
    float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    
	if(speed > 0) 
	{
		pos.x = constrain(pos.x, 0.0f, 1.0f);
		pos.y = constrain(pos.y, 0.0f, 1.0f);
		
		const float colorMult = 100;
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if( addColor )
		{
			fluidSolver.addColorAtIndex( index, Color :: white() * colorMult );
		}
		
		if( addForce )
		{
			fluidSolver.addForceAtIndex( index, vel * fluidInputVelocityMult );
		}
    }
}


///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzBlah :: draw ()
{
	ofSetColor( 0xFFFFFF );
	
	backgroundTexture->draw( 0, 0 );
	
	ofEnableAlphaBlending();
	
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzBlahParticle* particle;
		particle = particles[ i ];
		particle->draw();
	}
	
	ofDisableAlphaBlending();
}	

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzBlah :: keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}

void NawlzBlah :: keyReleased(int key)
{
	//
}

void NawlzBlah :: mouseMoved(int x, int y )
{
	Vec2f eventPos	= Vec2f( x, y );
	Vec2f mouseNorm	= Vec2f( eventPos ) / getWindowSize();
	Vec2f mouseVel	= Vec2f( eventPos - pMouse ) / getWindowSize();
	pMouse = eventPos;
	
	addToFluid( mouseNorm, mouseVel, false, true );
	
	fluidScale = 1.0;
}

void NawlzBlah :: mouseDragged(int x, int y, int button)
{
	//
}

void NawlzBlah :: mousePressed(int x, int y, int button)
{
	//
}

void NawlzBlah :: mouseReleased(int x, int y, int button)
{
	//
}

void NawlzBlah :: windowResized(int w, int h)
{
	//
}