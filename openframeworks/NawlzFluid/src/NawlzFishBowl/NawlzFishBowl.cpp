/*
 *  NawlzFishBowl.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzFishBowl.h"

NawlzFishBowl :: NawlzFishBowl ()
{
	backgroundTexture	= NULL;
	particleTexture		= NULL;
	bowlTexture			= NULL;
	bowlPixels			= NULL;
	
	bDrawBackground = true;
	bDrawParticles	= true;
	bDrawBowl		= false;
	bOverRoi		= false;
	bTouchDown		= false;
	
	roi.x		= 647;
	roi.y		= 80;
	roi.width	= 359;
	roi.height	= 359;
}

NawlzFishBowl :: ~NawlzFishBowl ()
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
	
	if( bowlTexture )
	{
		bowlTexture->clear();
		delete bowlTexture;
		bowlTexture = NULL;
		
		delete[] bowlPixels;
		bowlPixels = NULL;
	}
}

void NawlzFishBowl :: setup ()
{
	initFluid();
	createParticles();
}

void NawlzFishBowl :: initFluid ()
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

void NawlzFishBowl :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
	
	backgroundTextureXY.x = x;
	backgroundTextureXY.y = y;
}

void NawlzFishBowl :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

void NawlzFishBowl :: createBowlTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	bowlPixelDepth = 1;
	if( glType == GL_RGB )
		bowlPixelDepth = 3;
	else if( glType == GL_RGBA )
		bowlPixelDepth = 4;
	
	int t = width * height * bowlPixelDepth;
	bowlPixels = new unsigned char[ t ];
	for( int i=0; i<t; i++ )
	{
		bowlPixels[ i ] = pixels[ i ];
	}
	
	bowlTexture = new ofTexture();
	bowlTexture->allocate( width, height, glType );
	bowlTexture->loadData( bowlPixels, width, height, glType );
	
	bowlTextureXY.x = roi.x;
	bowlTextureXY.y = roi.y;
	
	bowlRect.x		= roi.x;
	bowlRect.y		= roi.y;
	bowlRect.width	= width;
	bowlRect.height	= height;
}

void NawlzFishBowl :: createParticles ()
{
	for( int i=0; i<100; i++ )
	{
		Vec2f vel;
		vel.set( 1, 0 );
		vel.rotate( ofRandom( -360, 360 ) * DEG_TO_RAD );
		vel *= ofRandom( 1.0, 3.0 );
		
		float gap = 0.4;
		
		Vec2f pos;
		pos.x = roi.x + ( roi.width  * gap ) + ofRandom( 0, roi.width  - ( roi.width  * gap * 2 ) );
		pos.y = roi.y + ( roi.height * gap ) + ofRandom( 0, roi.height - ( roi.height * gap * 2 ) );
		
		NawlzFishBowlParticle* particle;
		particle = new NawlzFishBowlParticle();
		particle->setLoc( pos.x, pos.y );
		particle->setVel( vel.x, vel.y );
		particle->setBounds( roi );
		particle->setImageBounds( bowlRect, bowlPixels, bowlPixelDepth );
		particle->setTexture( particleTexture );
		
		particles.push_back( particle );
	}
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzFishBowl :: update ()
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
	
	fluidScale *= 0.96;		// decay fluid scale.
	
	if( bDrawParticles )
	{
		updateParticles();
	}
}

void NawlzFishBowl :: updateParticles ()
{
	Vec2f vel;
	Vec2f roiCenter;
	roiCenter.set( roi.x + roi.width * 0.5, roi.y + roi.height * 0.5 );
	
	float maxVel;
	maxVel = 0.00025;
	maxVel = 0.0005;
	maxVel = 0.001;
//	maxVel = 0.003;
	
	int i = 0;
	int t = particles.size();
	for( i=0; i<t; i++ )
	{
		NawlzFishBowlParticle* particle;
		particle = particles[ i ];
		
		particle->update( 1 - fluidScale * 0.7 );
		
		//-- fluid forces.
		
		bool b1 = isPointInsideBowl( particle->loc.x, particle->loc.y );
//		bool b2 = isPointInsideROI( particle->loc.x, particle->loc.y );
		
		vel.set( 0, 0 );
		
		if( b1 )
		{
			float px = particle->loc.x / (float)ofGetWidth();
			float py = particle->loc.y / (float)ofGetHeight();
			
			Vec2f p( px, py );
			vel = fluidSolver.getVelocityAtPos( p );
			
			vel.limit( maxVel );
			vel *= 10000;
			vel *= fluidScale * fluidScale;
			vel *= particle->friction;
		}
		else
		{
//			Vec2f dir;
//			dir.x = roiCenter.x - particle->loc.x;
//			dir.y = roiCenter.y - particle->loc.y;
//			dir.normalize();
//			vel = dir;
		}
		
		float px = particle->loc.x + vel.x;
		float py = particle->loc.y + vel.y;
		
		if( !isPointInsideBowl( px, py ) )
			continue;
		
		particle->loc.x += vel.x;
		particle->loc.y += vel.y;
	}
}

bool NawlzFishBowl :: isPointInsideROI ( float x, float y )
{
	bool b1 = ( x >= roi.x );
	bool b2 = ( x <  roi.x + roi.width );
	bool b3 = ( y >= roi.y );
	bool b4 = ( y <  roi.y + roi.height );
	
	return ( b1 && b2 ) && ( b3 && b4 );
}

bool NawlzFishBowl :: isPointInsideBowl ( float x, float y )
{
	float px = ( x - roi.x ) / (float)roi.width;
	float py = ( y - roi.y ) / (float)roi.height;
	px = ofClamp( px, 0, 1 );
	py = ofClamp( py, 0, 1 );

	int iw	= bowlTexture->getWidth();
	int ih	= bowlTexture->getHeight();
	int ix	= px * iw;
	int iy	= py * ih;
	
	int p	= ( ( iy * iw ) + ix ) * bowlPixelDepth;
	
	return bowlPixels[ p ] < 255;
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzFishBowl :: draw ()
{
	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, 255 );
	
	if( backgroundTexture && bDrawBackground )
	{
		backgroundTexture->draw( backgroundTextureXY.x, backgroundTextureXY.y );
	}
	
	if( bowlTexture && bDrawBowl )
	{
		if( bOverRoi )
		{
			ofSetColor( 0, 0, 255, 255 );
		}
		
		bowlTexture->draw( bowlTextureXY.x, bowlTextureXY.y );
	}
	
	if( bDrawParticles )
	{
		drawParticles();
	}
}

void NawlzFishBowl :: drawParticles ()
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzFishBowlParticle* particle;
		particle = particles[ i ];
		particle->draw();
	}
}

///////////////////////////////////////////
//	FLUID.
///////////////////////////////////////////

void NawlzFishBowl :: addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce )
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
//	HANDLERS.
///////////////////////////////////////////

void NawlzFishBowl :: keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}

void NawlzFishBowl :: keyReleased(int key)
{
	//
}

void NawlzFishBowl :: mouseMoved(int x, int y )
{
	if( !isPointInsideBowl( x, y ) )
		return;
	
	Vec2f eventPos	= Vec2f( x, y );
	Vec2f mouseNorm	= Vec2f( eventPos ) / getWindowSize();
	Vec2f mouseVel	= Vec2f( eventPos - pMouse ) / getWindowSize();
	pMouse = eventPos;
	
	addToFluid( mouseNorm, mouseVel, false, true );
	
	fluidScale = 1.0;
}

void NawlzFishBowl :: mouseDragged(int x, int y, int button)
{
	//
}

void NawlzFishBowl :: mousePressed(int x, int y, int button)
{
	bTouchDown = true;
}

void NawlzFishBowl :: mouseReleased(int x, int y, int button)
{
	bTouchDown = false;
}

void NawlzFishBowl :: windowResized(int w, int h)
{
	//
}