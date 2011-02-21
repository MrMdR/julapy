/*
 *  NawlzSand.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 17/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzSand.h"

NawlzSand :: NawlzSand ()
{
	backgroundTexture	= NULL;
	particleTexture		= NULL;
}

NawlzSand :: ~NawlzSand ()
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
}

///////////////////////////////////////////
//	TEXTURES.
///////////////////////////////////////////

void NawlzSand :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzSand :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void NawlzSand :: setup	()
{
	initFluid();
	
	bounds.x		= 553;
	bounds.y		= 97;
	bounds.width	= 435;
	bounds.height	= 435;
	
	float extra = 0.2;
	bounds.x -= bounds.width  * extra;
	bounds.y -= bounds.height * extra;
	bounds.width  *= 1 + ( extra * 2 );
	bounds.height *= 1 + ( extra * 2 );
	
	for( int i=0; i<400; i++ )
	{
		addParticle();
	}
}

void NawlzSand :: initFluid ()
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

void NawlzSand :: addParticle ()
{
	Vec2f vel;
	vel.set( ofRandom( -1, 1 ), ofRandom( -1, 1 ) );
	vel.normalize();
	vel *= ofRandom( 1.0, 3.0 );
	
	float gap = 0.4;
	
	Vec2f pos;
	pos.x = bounds.x + ( bounds.width  * gap ) + ofRandom( 0, bounds.width  - ( bounds.width  * gap * 2 ) );
	pos.y = bounds.y + ( bounds.height * gap ) + ofRandom( 0, bounds.height - ( bounds.height * gap * 2 ) );
	
	NawlzSandParticle* particle;
	particle = new NawlzSandParticle();
	particle->setLoc( pos.x, pos.y );
	particle->setVel( vel.x, vel.y );
	particle->setTexture( particleTexture );
	particle->setBounds( bounds );
	particle->wanderTheta = ofRandom( 0, TWO_PI );
	
	particles.push_back( particle );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzSand :: update ()
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
		NawlzSandParticle* particle;
		particle = particles[ i ];
		
		particle->update( 1 - fluidScale );
		
		//--- calc fluid force.
		
		float px = particle->loc.x / (float)ofGetWidth();
		float py = particle->loc.y / (float)ofGetHeight();
		
		Vec2f p( px, py );
		fluidVel = fluidSolver.getVelocityAtPos( p );
		
		fluidVel.limit( maxVel );
		fluidVel *= 10000;
		fluidVel *= fluidScale * fluidScale;
		fluidVel *= particle->friction;
//		fluidVel *= ( lp < 0.1 ) ? 0 : 1.0;
//		fluidVel *= ( lp < 0.15 ) ? ( ( lp - 0.1 ) / 0.15 ) : 1.0;
		
		particle->loc.x += fluidVel.x;
		particle->loc.y += fluidVel.y;
	}
}

void NawlzSand :: addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce )
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

void NawlzSand :: draw ()
{
	ofSetColor( 0xFFFFFF );
	
	backgroundTexture->draw( 0, 0 );
	
	ofEnableAlphaBlending();
	
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzSandParticle* particle;
		particle = particles[ i ];
		particle->draw();
	}
	
	ofDisableAlphaBlending();
}	

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzSand :: keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}

void NawlzSand :: keyReleased(int key)
{
	//
}

void NawlzSand :: mouseMoved(int x, int y )
{
	Vec2f eventPos	= Vec2f( x, y );
	Vec2f mouseNorm	= Vec2f( eventPos ) / getWindowSize();
	Vec2f mouseVel	= Vec2f( eventPos - pMouse ) / getWindowSize();
	pMouse = eventPos;
	
	addToFluid( mouseNorm, mouseVel, false, true );
	
	fluidScale = 1.0;
}

void NawlzSand :: mouseDragged(int x, int y, int button)
{
	//
}

void NawlzSand :: mousePressed(int x, int y, int button)
{
	//
}

void NawlzSand :: mouseReleased(int x, int y, int button)
{
	//
}

void NawlzSand :: windowResized(int w, int h)
{
	//
}