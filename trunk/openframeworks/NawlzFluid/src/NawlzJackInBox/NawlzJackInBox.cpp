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
}

NawlzJackInBox :: ~NawlzJackInBox ()
{
    if( backgroundTexture )
    {
        backgroundTexture->clear();
        delete backgroundTexture;
        backgroundTexture = NULL;
    }
    
    NawlzJackInBoxPart* part;
    int t = parts.size();
    for( int i=0; i<t; i++ )
    {
        part = parts[ i ];
        delete part;
    }
    parts.clear();
}

void NawlzJackInBox :: setup ()
{
    //
}

void NawlzJackInBox :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzJackInBox :: createBionicPart ( unsigned char* pixels, int width, int height, int glType, const ofPoint& pos, const ofPoint& reg, float length, float rotation, bool jiggle )
{
    NawlzJackInBoxPart* part;
    part = new NawlzJackInBoxPart();
    part->tex = new ofTexture();
    part->tex->allocate( width, height, glType );
    part->tex->loadData( pixels, width, height, glType );
    part->texPos = pos;
    part->texReg = reg;
    
    if( jiggle )
        part->setup( pos, length, rotation );
    
    parts.push_back( part );
}

void NawlzJackInBox :: update ()
{
    mouseDir.x  = mouseCurr.x - mousePrev.x;
    mouseDir.y  = mouseCurr.y - mousePrev.y;
    mouseDir.limit( 15 );

    mousePrev   = mouseCurr;
    
    //----
    
    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        
        float d;
        d = ofDist( part->pEndInit.x, part->pEndInit.y, mouseCurr.x, mouseCurr.y );
        
        if( d < 100 )
            part->updateForce( mouseDir );
            
        part->update();
    }
}

void NawlzJackInBox :: draw ()
{
    ofEnableAlphaBlending();
    
    if( backgroundTexture )
    {
        ofSetColor( 0xFFFFFF );
        backgroundTexture->draw( 0, 0 );
    }
    
    NawlzJackInBoxPart* part;
    for( int i=0; i<parts.size(); i++ )
    {
        part = parts[ i ];
        part->draw( false );
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
    mouseCurr.x = x;
    mouseCurr.y = y;
    
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
