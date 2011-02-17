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
//	for( int i=0; i<10; i++ )
//	{
//		addParticle();
//	}
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzBlah :: update ()
{
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzBlahParticle* particle;
		particle = particles[ i ];
		
		particle->wanderChange		= wanderChange;
		particle->wanderRadius		= wanderRadius;
		particle->wanderDistance	= wanderDistance;
		particle->wanderEase		= wanderEase;
		
		particle->update();
		
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
	//
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