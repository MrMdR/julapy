//
//  NawlzJackInBox.cpp
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzInfodel.h"

NawlzInfodel :: NawlzInfodel ()
{
    backgroundTexture   = NULL;
    harleyOneTexture    = NULL;
    harleyTwoTexture    = NULL;
    particleTexture     = NULL;
    
    bounds.x        = 0;
    bounds.y        = 0;
    bounds.width    = ofGetWidth();
    bounds.height   = ofGetHeight();
    
    connectionLength    = 65;
    connectionAlpha     = 0.4;
    
    harleyMode = 0;
}

NawlzInfodel :: ~NawlzInfodel ()
{
    killParticles( particles0 );
    killParticles( particles1 );
    
    if( backgroundTexture )
    {
        backgroundTexture->clear();
        delete backgroundTexture;
        backgroundTexture = NULL;
    }

    if( harleyOneTexture )
    {
        harleyOneTexture->clear();
        delete harleyOneTexture;
        harleyOneTexture = NULL;
    }

    if( harleyTwoTexture )
    {
        harleyTwoTexture->clear();
        delete harleyTwoTexture;
        harleyTwoTexture = NULL;
    }
    
    if( particleTexture )
    {
        particleTexture->clear();
        delete particleTexture;
        particleTexture = NULL;
    }
}

void NawlzInfodel :: killParticles ( vector<NawlzInfodelParticle*>& particles )
{
    int t = particles.size();			// delete all particles.
    for( int i=0; i<t; i++ )
    {
        NawlzInfodelParticle* particle;
        particle = particles[ i ];
        
        particles.erase( particles.begin() + i );
        
        --i;
        --t;
        
        delete particle;
        particle = NULL;
    }
}

void NawlzInfodel :: setup ()
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
    
    //----
    
	for( int i=0; i<100; i++ )
		addParticle( particles0 );
    
	for( int i=0; i<100; i++ )
		addParticle( particles1 );
}

void NawlzInfodel :: addParticle ( vector<NawlzInfodelParticle*>& particles )
{
	Vec2f vel;
	vel.set( ofRandom( -1, 1 ), ofRandom( -1, 1 ) );
	vel.normalize();
	vel *= ofRandom( 1.0, 3.0 );
	
	float gap = 0.0;
	
	Vec2f pos;
	pos.x = bounds.x + ( bounds.width  * gap ) + ofRandom( 0, bounds.width  - ( bounds.width  * gap * 2 ) );
	pos.y = bounds.y + ( bounds.height * gap ) + ofRandom( 0, bounds.height - ( bounds.height * gap * 2 ) );
	
	NawlzInfodelParticle* particle;
	particle = new NawlzInfodelParticle();
	particle->pos.x = pos.x;
    particle->pos.y = pos.y;
	particle->vel.x = vel.x;
    particle->vel.y = vel.y;
	particle->wanderTheta       = ofRandom( 0, TWO_PI );
    particle->setTexture( particleTexture );
	
	particles.push_back( particle );
}

void NawlzInfodel :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzInfodel :: createHarleyOneTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    harleyOneTexture = new ofTexture();
	harleyOneTexture->allocate( width, height, glType );
	harleyOneTexture->loadData( pixels, width, height, glType );
    
    harleyOneRect.x        = x;
    harleyOneRect.y        = y;
    harleyOneRect.width    = width;
    harleyOneRect.height   = height;
}

void NawlzInfodel :: createHarleyTwoTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    harleyTwoTexture = new ofTexture();
	harleyTwoTexture->allocate( width, height, glType );
	harleyTwoTexture->loadData( pixels, width, height, glType );
    
    harleyTwoRect.x        = x;
    harleyTwoRect.y        = y;
    harleyTwoRect.width    = width;
    harleyTwoRect.height   = height;
}

void NawlzInfodel :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

void NawlzInfodel :: toggleHarley ()
{
    if( ++harleyMode >= 2 )
        harleyMode = 0;
}

void NawlzInfodel :: showHarleyOne ()
{
    harleyMode = 0;
}

void NawlzInfodel :: showHarleyTwo ()
{
    harleyMode = 1;
}

void NawlzInfodel :: update ()
{
    updateFluid();
	
	updateParticles( particles0, connections0 );
    updateParticles( particles1, connections1 );
}

void NawlzInfodel :: updateFluid ()
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
}

void NawlzInfodel :: updateParticles ( vector<NawlzInfodelParticle*>& particles, vector<ParticleConnection>& connections )
{
	Vec2f fluidVel;
	
	float maxVel;
	maxVel = 0.00025;
	maxVel = 0.0005;
	maxVel = 0.001;
	maxVel = 0.002;
	
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzInfodelParticle* particle;
		particle = particles[ i ];
		
		//--- calc fluid force.
		
		float px = particle->pos.x / (float)ofGetWidth();
		float py = particle->pos.y / (float)ofGetHeight();
		
		Vec2f p( px, py );
		fluidVel = fluidSolver.getVelocityAtPos( p );
		
		fluidVel.limit( maxVel );
		fluidVel *= 10000;
		fluidVel *= fluidScale * fluidScale;
		fluidVel *= particle->friction;
		
		particle->fluidVel.x = fluidVel.x;
		particle->fluidVel.y = fluidVel.y;
        
        particle->update( 1 - fluidScale );
	}
    
    connections.clear();
    
    for( int i=0; i<particles.size(); i++ )
    {
        for( int j=i+1; j<particles.size(); j++ )
        {
            NawlzInfodelParticle* p1 = particles[ i ];
            NawlzInfodelParticle* p2 = particles[ j ];
            
            float d;
            d = ( p1->pos.x - p2->pos.x ) * ( p1->pos.x - p2->pos.x ) + ( p1->pos.y - p2->pos.y ) * ( p1->pos.y - p2->pos.y );
            
            if( d < connectionLength * connectionLength )
            {
                ParticleConnection pc;
                pc.p1x = p1->pos.x;
                pc.p1y = p1->pos.y;
                pc.p2x = p2->pos.x;
                pc.p2y = p2->pos.y;
                
                pc.p1s = p1->sizeScale;
                pc.p2s = p2->sizeScale;
                
                connections.push_back( pc );
            }
        }
    }
}

void NawlzInfodel :: addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce )
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

void NawlzInfodel :: draw ()
{
    ofEnableAlphaBlending();
    
    ofSetColor( 255, 255, 255 );
    if( backgroundTexture )
    {
        backgroundTexture->draw( 0, 0 );
    }

    drawConnections( connections0 );
    drawParticles( particles0 );
    
    ofSetColor( 255, 255, 255 );
    if( harleyMode == 0 )
    {
        harleyOneTexture->draw( harleyOneRect.x, harleyOneRect.y );
    }
    else if( harleyMode == 1 )
    {
        harleyTwoTexture->draw( harleyTwoRect.x, harleyTwoRect.y );
    }
    
    drawConnections( connections1 );
    drawParticles( particles1 );
}

void NawlzInfodel :: drawParticles ( vector<NawlzInfodelParticle*>& particles  )
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzInfodelParticle* particle;
		particle = particles[ i ];
        particle->draw();
    }
}

void NawlzInfodel :: drawConnections ( vector<ParticleConnection>& connections )
{
    float* vert = new float[ 6 ];
    float* colr = new float[ 8 ];
    
    for( int i=0; i<connections.size(); i++ )
    {
        ParticleConnection& pc = connections[ i ];
        
        vert[ 0 ] = pc.p1x;
        vert[ 1 ] = pc.p1y;
        vert[ 2 ] = 0;
        vert[ 3 ] = pc.p2x;
        vert[ 4 ] = pc.p2y;
        vert[ 5 ] = 0;
        
        colr[ 0 ] = 1.0;
        colr[ 1 ] = 1.0;
        colr[ 2 ] = 1.0;
        colr[ 3 ] = connectionAlpha * pc.p1s;
        colr[ 4 ] = 1.0;
        colr[ 5 ] = 1.0;
        colr[ 6 ] = 1.0;
        colr[ 7 ] = connectionAlpha * pc.p2s;
        
        glEnableClientState( GL_COLOR_ARRAY );
        glColorPointer( 4, GL_FLOAT, 0, colr );
        
        glEnableClientState( GL_VERTEX_ARRAY );		
        glVertexPointer( 3, GL_FLOAT, 0, vert );
        
        glDrawArrays( GL_LINES, 0, 2 );
        
        glDisableClientState( GL_COLOR_ARRAY );
        glDisableClientState( GL_VERTEX_ARRAY );
    }
    
    delete[] vert;
    delete[] colr;
}

void NawlzInfodel :: keyPressed ( int key )
{
    //
}

void NawlzInfodel :: keyReleased ( int key )
{
    //
}

void NawlzInfodel :: mouseMoved ( int x, int y )
{
	Vec2f eventPos	= Vec2f( x, y );
	Vec2f mouseNorm	= Vec2f( eventPos ) / getWindowSize();
	Vec2f mouseVel	= Vec2f( eventPos - pMouse ) / getWindowSize();
	pMouse = eventPos;
	
	addToFluid( mouseNorm, mouseVel, false, true );
	
	fluidScale = 1.0;
}

void NawlzInfodel :: mouseDragged ( int x, int y, int button )
{
    //
}

void NawlzInfodel :: mousePressed ( int x, int y, int button )
{
    //
}

void NawlzInfodel :: mouseReleased ( int x, int y, int button )
{
    //
}

void NawlzInfodel :: windowResized ( int w, int h )
{
    //
}
