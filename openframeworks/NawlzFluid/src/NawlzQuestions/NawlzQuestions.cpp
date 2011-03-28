//
//  NawlzQuestions.cpp
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzQuestions.h"

NawlzQuestions :: NawlzQuestions ()
{
    backgroundTexture   = NULL;
    particleTexture     = NULL;
    
    bounds.x        = 0;
    bounds.y        = 0;
    bounds.width    = ofGetWidth();
    bounds.height   = ofGetHeight();
    
    boundsInner.x       = 622;
    boundsInner.y       = 262;
    boundsInner.width   = 234;
    boundsInner.height  = 173;
    
    boundsPoints.push_back( ofPoint( 611, 260 ) );
    boundsPoints.push_back( ofPoint( 920, 147 ) );
    boundsPoints.push_back( ofPoint( 857, 438 ) );
    boundsPoints.push_back( ofPoint( 622, 442 ) );
    
    boundsCenter = ofPoint( 745, 326 );
}

NawlzQuestions :: ~NawlzQuestions ()
{
    killParticles( particles0 );
    
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

void NawlzQuestions :: setup ()
{
	for( int i=0; i<30; i++ )
		addParticle( particles0 );
}

void NawlzQuestions :: killParticles ( vector<NawlzQuestionsParticle*>& particles )
{
    int t = particles.size();			// delete all particles.
    for( int i=0; i<t; i++ )
    {
        NawlzQuestionsParticle* particle;
        particle = particles[ i ];
        
        particles.erase( particles.begin() + i );
        
        --i;
        --t;
        
        delete particle;
        particle = NULL;
    }
}

void NawlzQuestions :: addParticle ( vector<NawlzQuestionsParticle*>& particles )
{
	Vec2f vel;
	vel.set( ofRandom( -1, 1 ), ofRandom( -1, 1 ) );
	vel.normalize();
	vel *= ofRandom( 1.0, 3.0 );
	
	float gap = 0.0;
	
	Vec2f pos;
	pos.x = boundsInner.x + ( boundsInner.width  * gap ) + ofRandom( 0, boundsInner.width  - ( boundsInner.width  * gap * 2 ) );
	pos.y = boundsInner.y + ( boundsInner.height * gap ) + ofRandom( 0, boundsInner.height - ( boundsInner.height * gap * 2 ) );
	
	NawlzQuestionsParticle* particle;
	particle = new NawlzQuestionsParticle();
	particle->pos.x = pos.x;
    particle->pos.y = pos.y;
	particle->vel.x = vel.x;
    particle->vel.y = vel.y;
	particle->wanderTheta       = ofRandom( 0, TWO_PI );
    particle->setTexture( particleTexture );
    particle->setBounds( boundsPoints );
    particle->setBoundsCenter( boundsCenter );
	
	particles.push_back( particle );
}

void NawlzQuestions :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzQuestions :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

void NawlzQuestions :: update ()
{
	updateParticles( particles0 );
}

void NawlzQuestions :: updateParticles ( vector<NawlzQuestionsParticle*>& particles )
{
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzQuestionsParticle* particle;
		particle = particles[ i ];
        particle->update( 1 );
	}
}

void NawlzQuestions :: draw ()
{
    ofEnableAlphaBlending();
    
    ofSetColor( 255, 255, 255 );
    if( backgroundTexture )
    {
        backgroundTexture->draw( 0, 0 );
    }

    drawParticles( particles0 );
}

void NawlzQuestions :: drawParticles ( vector<NawlzQuestionsParticle*>& particles  )
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzQuestionsParticle* particle;
		particle = particles[ i ];
        particle->draw();
    }
}

void NawlzQuestions :: keyPressed ( int key )
{
    //
}

void NawlzQuestions :: keyReleased ( int key )
{
    //
}

void NawlzQuestions :: mouseMoved ( int x, int y )
{
	//
}

void NawlzQuestions :: mouseDragged ( int x, int y, int button )
{
    //
}

void NawlzQuestions :: mousePressed ( int x, int y, int button )
{
    //
}

void NawlzQuestions :: mouseReleased ( int x, int y, int button )
{
    //
}

void NawlzQuestions :: windowResized ( int w, int h )
{
    //
}
