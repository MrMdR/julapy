//
//  NawlzGardener.cpp
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzGardener.h"

NawlzGardener :: NawlzGardener ( int appIndex )
{
    this->appIndex = appIndex;
    
    backgroundTexture   = NULL;
    particle01Texture   = NULL;
    particle02Texture   = NULL;
}

NawlzGardener :: ~NawlzGardener ()
{
    killParticles();
    
    if( backgroundTexture )
    {
        backgroundTexture->clear();
        delete backgroundTexture;
        backgroundTexture = NULL;
    }
    
    if( particle01Texture )
    {
        particle01Texture->clear();
        delete particle01Texture;
        particle01Texture = NULL;
    }
    
    if( particle02Texture )
    {
        particle02Texture->clear();
        delete particle02Texture;
        particle02Texture = NULL;
    }
}

void NawlzGardener :: setup ()
{
	//
}

void NawlzGardener :: killParticles ()
{
    int t = particles.size();			// delete all particles.
    for( int i=0; i<t; i++ )
    {
        NawlzGardenerParticle* particle;
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

void NawlzGardener :: addParticle0 ()
{
    ofRectangle bounds;
    bounds.x = 680;
    bounds.y = 370;
    bounds.width  = 149;
    bounds.height = 175;
    
    vector<ofPoint> boundsPoints;
    boundsPoints.push_back( ofPoint( 831, 369 ) );
    boundsPoints.push_back( ofPoint( 828, 543 ) );
    boundsPoints.push_back( ofPoint( 697, 603 ) );
    boundsPoints.push_back( ofPoint( 586, 602 ) );
    boundsPoints.push_back( ofPoint( 652, 370 ) );

    ofPoint boundsCenter;
    boundsCenter = ofPoint( 586, 601 );
    
    Vec2f pos;
	pos.x = bounds.x + bounds.width - 10;
	pos.y = bounds.y + ofRandom( 0, bounds.height );
    
	Vec2f vel;
	vel.set( -1, 0 );
	vel.normalize();
	vel *= ofRandom( 1.0, 3.0 );
    
    float theta;
    float thetaCenter;
    float thetaClamp;
    
    theta       = -PI;
    thetaCenter = -PI;
    thetaClamp  = PI * 0.25;
	
	NawlzGardenerParticle* particle;
	particle = new NawlzGardenerParticle();
	particle->pos.x = pos.x;
    particle->pos.y = pos.y;
	particle->vel.x = vel.x;
    particle->vel.y = vel.y;
	particle->wanderTheta       = theta;
    particle->wanderMaxSpeed    = ofRandom( 0.5, 1.5 );
    particle->setBounds( boundsPoints );
    particle->setBoundsCenter( boundsCenter );
    particle->setThetaClamp( thetaCenter, thetaClamp );
    
    if( ofRandom( 0.0, 1.0 ) > 0.5 )
        particle->setTexture( particle01Texture );
    else
        particle->setTexture( particle02Texture );
	
	particles.push_back( particle );
}

void NawlzGardener :: addParticle1 ()
{
    vector<ofPoint> boundsPoints;
    boundsPoints.push_back( ofPoint( 946, 265 ) );
    boundsPoints.push_back( ofPoint( 949, 593 ) );
    boundsPoints.push_back( ofPoint( 511, 589 ) );
    boundsPoints.push_back( ofPoint( 485, 529 ) );
    boundsPoints.push_back( ofPoint( 509, 279 ) );
    
    ofPoint boundsCenter;
    boundsCenter = ofPoint( 946, 265 );
    
    Vec2f pos;
    Vec2f p1( 540, 314 );
    Vec2f p2( 514, 503 );
    float s = ofRandom( 0.0, 1.0 );
    pos = p1 + ( p2 - p1 ) * s;
    
    Vec2f vel( 1, 0 );
	vel *= ofRandom( 1.0, 3.0 );
	
	NawlzGardenerParticle* particle;
	particle = new NawlzGardenerParticle();
	particle->pos.x = pos.x;
    particle->pos.y = pos.y;
	particle->vel.x = vel.x;
    particle->vel.y = vel.y;
	particle->wanderTheta       = 0;
    particle->wanderMaxSpeed    = ofRandom( 1.0, 2.0 );
    particle->setBounds( boundsPoints );
    particle->setBoundsCenter( boundsCenter );
    particle->setThetaClamp( 0, PI * 0.35 );
    
    if( ofRandom( 0.0, 1.0 ) > 0.5 )
        particle->setTexture( particle01Texture );
    else
        particle->setTexture( particle02Texture );
	
	particles.push_back( particle );
}

void NawlzGardener :: addParticle2 ()
{
    float r = ofRandom( 0.0, 1.0 );
    
    vector<ofPoint> boundsPoints;
    ofPoint boundsCenter;
    Vec2f pos;
    Vec2f vel;
    
    float theta;
    float thetaCenter;
    float thetaClamp;
    
    if( r < 0.6 )       // brain.
    {
        boundsPoints.push_back( ofPoint( 965, 6 ) );
        boundsPoints.push_back( ofPoint( 830, 262 ) );
        boundsPoints.push_back( ofPoint( 745, 262 ) );
        boundsPoints.push_back( ofPoint( 649, 6 ) );
        
        boundsCenter = ofPoint( 796, 15 );
        
        Vec2f p1( 758, 243 );
        Vec2f p2( 827, 243 );
        float s = ofRandom( 0.0, 1.0 );
        pos = p1 + ( p2 - p1 ) * s;
        
        vel.set( 0, -1 );
        vel *= ofRandom( 1.0, 3.0 );
        
        theta       = -PI * 0.5;
        thetaCenter = -PI * 0.5;
        thetaClamp  = PI * 0.25;
    }
    else if( r < 0.8 )     // right ear.
    {
        boundsPoints.push_back( ofPoint( 1023, 201 ) );
        boundsPoints.push_back( ofPoint( 833, 326 ) );
        boundsPoints.push_back( ofPoint( 833, 285 ) );
        boundsPoints.push_back( ofPoint( 977, 4 ) );
        
        boundsCenter = ofPoint( 977, 4 );
        
        Vec2f p1( 845, 265 );
        Vec2f p2( 841, 311 );
        float s = ofRandom( 0.0, 1.0 );
        pos = p1 + ( p2 - p1 ) * s;
        
        vel.set( 1, -1 );
        vel.normalize();
        vel *= ofRandom( 1.0, 3.0 );
        
        theta       = -PI * 0.25;
        thetaCenter = -PI * 0.25;
        thetaClamp  = PI * 0.25;
    }
    else if( r < 1.0 )      // left ear.
    {
        boundsPoints.push_back( ofPoint( 632, 4 ) );
        boundsPoints.push_back( ofPoint( 745, 286 ) );
        boundsPoints.push_back( ofPoint( 745, 327 ) );
        boundsPoints.push_back( ofPoint( 555, 202 ) );
        
        boundsCenter = ofPoint( 632, 4 );
        
        Vec2f p1( 730, 269 );
        Vec2f p2( 726, 315 );
        float s = ofRandom( 0.0, 1.0 );
        pos = p1 + ( p2 - p1 ) * s;
        
        vel.set( -1, -1 );
        vel.normalize();
        vel *= ofRandom( 1.0, 3.0 );
        
        theta       = -PI * 0.75;
        thetaCenter = -PI * 0.75;
        thetaClamp  = PI * 0.25;
    }
	
	NawlzGardenerParticle* particle;
	particle = new NawlzGardenerParticle();
	particle->pos.x = pos.x;
    particle->pos.y = pos.y;
	particle->vel.x = vel.x;
    particle->vel.y = vel.y;
	particle->wanderTheta       = theta;
    particle->wanderMaxSpeed    = ofRandom( 1.0, 2.0 );
    particle->setBounds( boundsPoints );
    particle->setBoundsCenter( boundsCenter );
    particle->setThetaClamp( thetaCenter, thetaClamp );
    
    if( ofRandom( 0.0, 1.0 ) > 0.5 )
        particle->setTexture( particle01Texture );
    else
        particle->setTexture( particle02Texture );
	
	particles.push_back( particle );
}

void NawlzGardener :: addParticle3 ()
{
    float r = ofRandom( 0.0, 1.0 );
    
    vector<ofPoint> boundsPoints;
    ofPoint boundsCenter;
    Vec2f pos;
    Vec2f vel;
    
    float theta;
    float thetaCenter;
    float thetaClamp;
    
    boundsPoints.push_back( ofPoint( 1009, 0 ) );
    boundsPoints.push_back( ofPoint( 940, 373 ) );
    boundsPoints.push_back( ofPoint( 800, 369 ) );
    boundsPoints.push_back( ofPoint( 572, 0 ) );
    
    boundsCenter = ofPoint( 652, 0 );
    
    Vec2f p1( 792, 329 );
    Vec2f p2( 917, 347 );
    float s = ofRandom( 0.0, 1.0 );
    pos = p1 + ( p2 - p1 ) * s;
    
    vel.set( 0, -1 );
    vel *= ofRandom( 1.0, 3.0 );
    
    theta       = -PI * 0.5;
    thetaCenter = -PI * 0.5;
    thetaClamp  = PI * 0.25;
    
	NawlzGardenerParticle* particle;
	particle = new NawlzGardenerParticle();
	particle->pos.x = pos.x;
    particle->pos.y = pos.y;
	particle->vel.x = vel.x;
    particle->vel.y = vel.y;
	particle->wanderTheta       = theta;
    particle->wanderMaxSpeed    = ofRandom( 1.0, 2.0 );
    particle->setBounds( boundsPoints );
    particle->setBoundsCenter( boundsCenter );
    particle->setThetaClamp( thetaCenter, thetaClamp );
    
    if( ofRandom( 0.0, 1.0 ) > 0.5 )
        particle->setTexture( particle01Texture );
    else
        particle->setTexture( particle02Texture );
	
	particles.push_back( particle );
}

/////////////////////////////////////////////
//  TEXTURES.
/////////////////////////////////////////////

void NawlzGardener :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzGardener :: createParticle01Texture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    particle01Texture = new ofTexture();
	particle01Texture->allocate( width, height, glType );
	particle01Texture->loadData( pixels, width, height, glType );
}

void NawlzGardener :: createParticle02Texture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    particle02Texture = new ofTexture();
	particle02Texture->allocate( width, height, glType );
	particle02Texture->loadData( pixels, width, height, glType );
}

/////////////////////////////////////////////
//  UPDATE.
/////////////////////////////////////////////

void NawlzGardener :: update ()
{
    if( appIndex == NAWLZ_GARDENER_APP_INDEX_0 )
        updateParticles0();
    else if( appIndex == NAWLZ_GARDENER_APP_INDEX_1 )
        updateParticles1();
    else if( appIndex == NAWLZ_GARDENER_APP_INDEX_2 )
        updateParticles2();
    else if( appIndex == NAWLZ_GARDENER_APP_INDEX_3 )
        updateParticles3();
    
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzGardenerParticle* particle;
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

void NawlzGardener :: updateParticles0 ()
{
    float particleRate;
    particleRate = 0.75;
    
    if( ofRandom( 0.0, 1.0 ) < particleRate )
        addParticle0();
}

void NawlzGardener :: updateParticles1 ()
{
    float particleRate;
    particleRate = 0.5;
    
    if( ofRandom( 0.0, 1.0 ) < particleRate )
        addParticle1();
}

void NawlzGardener :: updateParticles2 ()
{
    float particleRate;
    particleRate = 1.0;
    
    if( ofRandom( 0.0, 1.0 ) < particleRate )
        addParticle2();
}

void NawlzGardener :: updateParticles3 ()
{
    float particleRate;
    particleRate = 1.0;
    
    if( ofRandom( 0.0, 1.0 ) < particleRate )
        addParticle3();
}

/////////////////////////////////////////////
//  DRAW.
/////////////////////////////////////////////

void NawlzGardener :: draw ()
{
    ofEnableAlphaBlending();
    
    ofSetColor( 255, 255, 255 );
    if( backgroundTexture )
    {
        backgroundTexture->draw( 0, 0 );
    }
    
    drawParticles();
}

void NawlzGardener :: drawParticles ()
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzGardenerParticle* particle;
		particle = particles[ i ];
        particle->draw();
    }
}

/////////////////////////////////////////////
//  HANDLERS.
/////////////////////////////////////////////

void NawlzGardener :: keyPressed ( int key )
{
    //
}

void NawlzGardener :: keyReleased ( int key )
{
    //
}

void NawlzGardener :: mouseMoved ( int x, int y )
{
	//
}

void NawlzGardener :: mouseDragged ( int x, int y, int button )
{
    //
}

void NawlzGardener :: mousePressed ( int x, int y, int button )
{
    //
}

void NawlzGardener :: mouseReleased ( int x, int y, int button )
{
    //
}

void NawlzGardener :: windowResized ( int w, int h )
{
    //
}
