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
	//
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
	bounds.x		= 553;
	bounds.y		= 97;
	bounds.width	= 435;
	bounds.height	= 435;
	
	float extra = 0.2;
	bounds.x -= bounds.width  * extra;
	bounds.y -= bounds.height * extra;
	bounds.width  *= 1 + ( extra * 2 );
	bounds.height *= 1 + ( extra * 2 );
	
	for( int i=0; i<300; i++ )
	{
		addParticle();
	}
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
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzSandParticle* particle;
		particle = particles[ i ];
		particle->update();
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
	//
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