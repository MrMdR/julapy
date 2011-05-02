//
//  NawlzDandelion.cpp
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzDandelion.h"

NawlzDandelion :: NawlzDandelion ()
{
    this->appIndex = appIndex;
    
    backgroundTexture   = NULL;
    particleTexture     = NULL;
}

NawlzDandelion :: ~NawlzDandelion ()
{
    killParticles();
    
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

void NawlzDandelion :: setup ()
{
	for( int i=0; i<300; i++ )
    {
        addParticle();
    }
}

void NawlzDandelion :: killParticles ()
{
    int t = particles.size();			// delete all particles.
    for( int i=0; i<t; i++ )
    {
        NawlzDandelionParticle* particle;
        particle = particles[ i ];
        
        particles.erase( particles.begin() + i );
        
        --i;
        --t;
        
        delete particle;
        particle = NULL;
    }
}

/////////////////////////////////////////////
//  ADD PARTICLES.
/////////////////////////////////////////////

void NawlzDandelion :: addParticle ()
{
	NawlzDandelionParticle* particle;
	particle = new NawlzDandelionParticle();
    particle->setTexture( particleTexture );
	
	particles.push_back( particle );
}

/////////////////////////////////////////////
//  TEXTURES.
/////////////////////////////////////////////

void NawlzDandelion :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzDandelion :: createFatLadyTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    fatLadyTexture = new ofTexture();
	fatLadyTexture->allocate( width, height, glType );
	fatLadyTexture->loadData( pixels, width, height, glType );
    
    fatLadyTexturePos.x = x;
    fatLadyTexturePos.y = y;
}

void NawlzDandelion :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

/////////////////////////////////////////////
//  UPDATE.
/////////////////////////////////////////////

void NawlzDandelion :: update ()
{
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzDandelionParticle* particle;
		particle = particles[ i ];
        particle->update( 1 );
        
        if( !particle->bAlive )
        {
            particles.erase( particles.begin() + i );
            
            delete particle;
            particle = NULL;
            
            --i;
            --t;
        }
	}
}

/////////////////////////////////////////////
//  DRAW.
/////////////////////////////////////////////

void NawlzDandelion :: draw ()
{
    ofEnableAlphaBlending();
    
    ofSetColor( 255, 255, 255 );
    if( backgroundTexture )
    {
        backgroundTexture->draw( 0, 0 );
    }
    
    float cx = ofGetWidth()  * 0.5;
    float cy = ofGetHeight() * 0.5 - 50;
    float rx = -15;
    float rz = -15;
    
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
    glPushMatrix();
    glTranslatef( cx, cy, 0 );
    glRotatef( rx, 1, 0, 0 );
    glRotatef( rz, 0, 0, 1 );
    {
        drawParticlesBehind();
    }
    glPopMatrix();
    
    ofEnableAlphaBlending();
    
    ofSetColor( 255, 255, 255 );
    if( fatLadyTexture )
    {
        fatLadyTexture->draw( fatLadyTexturePos.x, fatLadyTexturePos.y );
    }
    
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
    glPushMatrix();
    glTranslatef( cx, cy, 0 );
    glRotatef( rx, 1, 0, 0 );
    glRotatef( rz, 0, 0, 1 );
    {
        drawParticlesFront();
    }
    glPopMatrix();
    
    ofEnableAlphaBlending();
}

void NawlzDandelion :: drawParticlesBehind ()
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzDandelionParticle* particle;
		particle = particles[ i ];
        particle->drawBehind();
    }
}

void NawlzDandelion :: drawParticlesFront ()
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzDandelionParticle* particle;
		particle = particles[ i ];
        particle->drawFront();
    }
}

/////////////////////////////////////////////
//  HANDLERS.
/////////////////////////////////////////////

void NawlzDandelion :: keyPressed ( int key )
{
    //
}

void NawlzDandelion :: keyReleased ( int key )
{
    //
}

void NawlzDandelion :: mouseMoved ( int x, int y )
{
	//
}

void NawlzDandelion :: mouseDragged ( int x, int y, int button )
{
    //
}

void NawlzDandelion :: mousePressed ( int x, int y, int button )
{
    //
}

void NawlzDandelion :: mouseReleased ( int x, int y, int button )
{
    //
}

void NawlzDandelion :: windowResized ( int w, int h )
{
    //
}