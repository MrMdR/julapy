//
//  NawlzDandelionParticle.cpp
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzDandelionParticle.h"

NawlzDandelionParticle :: NawlzDandelionParticle ()
{
    tex = NULL;
    
    radius      = 250;
    rotY        = ofRandom( 0, TWO_PI );
    rotYInc     = ofRandom( TWO_PI * 0.001, TWO_PI * 0.002 );
    
    oscAng      = ofRandom( 0, TWO_PI );
    oscAngInc   = ofRandom( TWO_PI * 0.005, TWO_PI * 0.010 );
    oscDist     = ofRandom( 10, 20 );
    
    sizeScale   = 1.0;
    alpha       = 0.0;
    alphaMax    = ofRandom( 0.6, 1.0 );
    
    rotation    = ofRandom( 0, TWO_PI );
    rotationInc = ofRandom( TWO_PI * 0.01, TWO_PI * 0.02 );
    rotationInc *= ( ofRandom( 0.0, 1.0 ) < 0.5 ) ? -1 : 1;
    
    width       = 2 * sizeScale;
    height      = 2 * sizeScale;
    widthHalf   = width  * 0.5;
    heightHalf  = height * 0.5;
    
    bAlive      = true;
    bFadeOut    = false;
}

NawlzDandelionParticle :: ~NawlzDandelionParticle ()
{
    //
}

void NawlzDandelionParticle :: setup ()
{
    //
}

void NawlzDandelionParticle :: setTexture ( ofTexture* tex )
{
    this->tex   = tex;
    
    width       = tex->getWidth()  * sizeScale;
    height      = tex->getHeight() * sizeScale;
    widthHalf   = width  * 0.5;
    heightHalf  = height * 0.5;
}

void NawlzDandelionParticle :: update ( float forceScale )
{
    rotY += rotYInc;
    oscAng += oscAngInc;

    pos.y = oscDist * sin( oscAng );
    pos.x = radius * sin( rotY );
    pos.z = radius * cos( rotY );
    
    rotation += rotationInc;
    
    if( alpha < 1.0 )
    {
        alpha += 0.05;
        alpha = MIN( alpha, 1.0 );
    }
}

void NawlzDandelionParticle :: draw ()
{
    glPushMatrix();
    glTranslatef( pos.x, pos.y, pos.z );
    glRotatef( rotation * RAD_TO_DEG, 0, 0, 1 );
    
    if( tex )
    {
        ofSetColor( 255, 255, 255, alpha * 255 );
        tex->draw( -widthHalf, -heightHalf, width, height );
    }
    else
    {
        ofFill();
        ofSetColor( 255, 255, 255, alpha * 255 );
        ofRect( -widthHalf, -heightHalf, width, height );
    }
    
    glPopMatrix();
}

void NawlzDandelionParticle :: drawBehind ()
{
    if( pos.z >= 0 )
        return;
    
    glPushMatrix();
    glTranslatef( pos.x, pos.y, pos.z );
    glRotatef( rotation * RAD_TO_DEG, 0, 0, 1 );
    
    if( tex )
    {
        ofSetColor( 255, 255, 255, alpha * 255 );
        tex->draw( -widthHalf, -heightHalf, width, height );
    }
    else
    {
        ofFill();
        ofSetColor( 255, 255, 255, alpha * 255 );
        ofRect( -widthHalf, -heightHalf, width, height );
    }
    
    glPopMatrix();
}

void NawlzDandelionParticle :: drawFront ()
{
    if( pos.z < 0 )
        return;
    
    glPushMatrix();
    glTranslatef( pos.x, pos.y, pos.z );
    glRotatef( rotation * RAD_TO_DEG, 0, 0, 1 );
    
    if( tex )
    {
        ofSetColor( 255, 255, 255, alpha * 255 );
        tex->draw( -widthHalf, -heightHalf, width, height );
    }
    else
    {
        ofFill();
        ofSetColor( 255, 255, 255, alpha * 255 );
        ofRect( -widthHalf, -heightHalf, width, height );
    }
    
    glPopMatrix();
}