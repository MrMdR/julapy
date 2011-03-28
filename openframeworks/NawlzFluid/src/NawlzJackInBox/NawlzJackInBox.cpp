//
//  NawlzJackInBox.cpp
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzJackInBox.h"

NawlzJackInBox :: NawlzJackInBox ()
{
    backgroundTexture   = NULL;
    tongueTex           = NULL;
}

NawlzJackInBox :: ~NawlzJackInBox ()
{
    if( backgroundTexture )
    {
        backgroundTexture->clear();
        delete backgroundTexture;
        backgroundTexture = NULL;
    }
    
    if( tongueTex )
    {
        tongueTex->clear();
        delete tongueTex;
        tongueTex = NULL;
    }
}

void NawlzJackInBox :: setup ()
{
    initFluid();
    
    NawlzJackInBoxPart* part;
    
    int t = 6;
    for( int i=0; i<t; i++ )
    {
        float p = i / (float)t;
        
        part = new NawlzJackInBoxPart();
        part->setup( ofPoint( ofGetWidth() * 0.25, ofGetHeight() * 0.25 ), 100, p * 360 );
        parts.push_back( part );
    }
    
    part = new NawlzJackInBoxPart();
    part->setup( ofPoint( 643, 427 ), ofRandom( 50, 100 ), 306 );
    parts.push_back( part );
    
    part = new NawlzJackInBoxPart();
    part->setup( ofPoint( 642, 331 ), ofRandom( 50, 100 ), 249 );
    parts.push_back( part );
    
    part = new NawlzJackInBoxPart();
    part->setup( ofPoint( 751, 422 ), ofRandom( 50, 100 ), 81 );
    parts.push_back( part );
    
    part = new NawlzJackInBoxPart();
    part->setup( ofPoint( 759, 332 ), ofRandom( 50, 100 ), 125 );
    parts.push_back( part );
    
    part = new NawlzJackInBoxPart();
    part->setup( ofPoint( 759, 332 ), ofRandom( 50, 100 ), 97 );
    parts.push_back( part );
    
    part = new NawlzJackInBoxPart();
    part->setup( ofPoint( 723, 308 ), ofRandom( 50, 100 ), 90 );
    parts.push_back( part );
}

void NawlzJackInBox :: initFluid ()
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

void NawlzJackInBox :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzJackInBox :: createTongueTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    tongueTex = new ofTexture();
	tongueTex->allocate( width, height, glType );
	tongueTex->loadData( pixels, width, height, glType );
    
    tongueTexRect.x         = 518;
    tongueTexRect.y         = 161;
    tongueTexRect.width     = width;
    tongueTexRect.height    = height;
    
    tongueTexRegPoint.x     = 20;
    tongueTexRegPoint.y     = 10;
}

void NawlzJackInBox :: update ()
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
    
    //---
    
	Vec2f fluidVel;
	
	float maxVel;
	maxVel = 0.00025;
	maxVel = 0.0005;
	maxVel = 0.001;
//	maxVel = 0.002;
    
    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        
		float px = part->pEnd.x / (float)ofGetWidth();
		float py = part->pEnd.y / (float)ofGetHeight();
		
		Vec2f p( px, py );
		fluidVel = fluidSolver.getVelocityAtPos( p );
		
		fluidVel.limit( maxVel );
		fluidVel *= 10000;
		fluidVel *= fluidScale * fluidScale;
        
        part->updateForce( ofPoint( fluidVel.x, fluidVel.y ) );
        part->update();
    }
}

void NawlzJackInBox :: addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce )
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

void NawlzJackInBox :: draw ()
{
    if( backgroundTexture )
    {
        ofSetColor( 0xFFFFFF );
        backgroundTexture->draw( 0, 0 );
    }
    
    drawTexRotated( tongueTex, ofPoint( tongueTexRect.x, tongueTexRect.y ), ofPoint( -tongueTexRegPoint.x, -tongueTexRegPoint.y ), ofGetFrameNum() );

    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        part->draw();
    }
}

void NawlzJackInBox :: drawTexRotated ( ofTexture* tex, const ofPoint& pos, const ofPoint& reg, float angle )
{
    if( tex )
    {
        glPushMatrix();
        glTranslatef( pos.x, pos.y, 0 );
        
        glPushMatrix();
        glRotatef( angle, 0, 0, 1 );
        glTranslatef( reg.x, reg.y, 0 );
        
        ofSetColor( 0xFFFFFF );
        tex->draw( 0, 0 );
        
        glPopMatrix();
        
        ofFill();
        ofSetColor( 0x00FF00 );
        ofRect( -2, -2, 4, 4 );
        
        glPopMatrix();
    }
}

void NawlzJackInBox :: keyPressed ( int key )
{
    //
}

void NawlzJackInBox :: keyReleased ( int key )
{
    //
}

void NawlzJackInBox :: mouseMoved ( int x, int y )
{
	Vec2f eventPos	= Vec2f( x, y );
	Vec2f mouseNorm	= Vec2f( eventPos ) / getWindowSize();
	Vec2f mouseVel	= Vec2f( eventPos - pMouse ) / getWindowSize();
	pMouse = eventPos;
	
	addToFluid( mouseNorm, mouseVel, false, true );
	
	fluidScale = 1.0;
    
    //----
    
    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        part->mouseMoved( x, y );
    }
}

void NawlzJackInBox :: mouseDragged ( int x, int y, int button )
{
    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        part->mouseDragged( x, y, button );
    }
}

void NawlzJackInBox :: mousePressed ( int x, int y, int button )
{
    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        part->mousePressed( x, y, button );
    }
}

void NawlzJackInBox :: mouseReleased ( int x, int y, int button )
{
    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        part->mouseReleased( x, y, button );
    }
}

void NawlzJackInBox :: windowResized ( int w, int h )
{
    //
}
