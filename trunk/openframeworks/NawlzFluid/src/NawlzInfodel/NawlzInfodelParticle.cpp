//
//  NawlzInfodelParticle.cpp
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzInfodelParticle.h"

NawlzInfodelParticle :: NawlzInfodelParticle ()
{
    tex = NULL;

    float a, b, c, s;
    a = 1.0;
    b = 3.0;
    c = ofRandom( a, b );
    s = ( c - a ) / ( b - a );
    
	wanderTheta			= 0;
	wanderRadius		= 16.0;
	wanderDistance		= 60.0;
	wanderChange		= 0.25;
	wanderEase			= 0.2;
	wanderEaseTarget	= wanderEase;
    wanderMaxSpeed      = c;
    
    sizeScale           = ofClamp( s, 0.2, 1.0 );
    friction            = ofRandom( 0.5, 1.0 );
    
    width       = 2;
    height      = 2;
    widthHalf   = width  * 0.5;
    heightHalf  = height * 0.5;
    
    bounds.x        = 0;
    bounds.y        = 0;
    bounds.width    = ofGetWidth();
    bounds.height   = ofGetHeight();
}

NawlzInfodelParticle :: ~NawlzInfodelParticle ()
{
    //
}

void NawlzInfodelParticle :: setup ()
{
    //
}

void NawlzInfodelParticle :: setTexture ( ofTexture* tex )
{
    this->tex = tex;
    
    width       = tex->getWidth()  * sizeScale;
    height      = tex->getHeight() * sizeScale;
    widthHalf   = width  * 0.5;
    heightHalf  = height * 0.5;
}

void NawlzInfodelParticle :: update ( float forceScale )
{
    wander();
    
    vel = 0;
    vel += wanderVel ;
    vel += fluidVel * ( 1 - forceScale );
    pos += vel;
    
    if( pos.y < heightHalf )
    {
        float h;
        h = ofGetHeight();
        h *= ofRandom( 1.1, 2.0 );
        
        pos.y += h;
        pos.x = ofRandom( 0, ofGetWidth() );
    }
}

void NawlzInfodelParticle :: wander()
{
    float theta;
    theta = wanderTheta - (int)( wanderTheta / TWO_PI ) * TWO_PI;
    theta += PI * 0.5;

    float a = -PI * 0.5;        // start by pointing in the upward direction.
    float d = PI * 0.5;         // then limit to the left and right by d.
    d *= 0.8;
    
	wanderTheta += ofRandom( -wanderChange, wanderChange );     // Randomly change wander theta
//    wanderTheta = ofClamp( wanderTheta, a - d, a + d );
	
	wanderCircle = wanderVel;			// Start with velocity
	wanderCircle.normalize();			// Normalize to get heading
	wanderCircle *= wanderDistance;		// Multiply by distance
	wanderCircle += pos;				// Make it relative to boid's locationx
	
	bool isInside;
	isInside = checkIsInsideBounds( wanderCircle );
	if( !isInside )
		constrainToBounds( wanderCircle );
	
	wanderEaseTarget = isInside ? 0.2 : 1.0;
	wanderEase		+= ( wanderEaseTarget - wanderEase ) * 0.3;
	
	wanderCircleOffSet	= ofxVec2f( wanderRadius * cos( wanderTheta ), wanderRadius * sin( wanderTheta ) );
	wanderCircleTarget	= wanderCircle + wanderCircleOffSet;
	
	ofxVec2f steerVec;
	steerVec		= wanderCircleTarget - pos;		// A vector pointing from the location to the target
	steerVec.normalize();
	steerVec *= wanderMaxSpeed;
	
	wanderVel.x += ( steerVec.x - wanderVel.x ) * wanderEase;
	wanderVel.y += ( steerVec.y - wanderVel.y ) * wanderEase;
}

bool NawlzInfodelParticle :: checkIsInsideBounds ( const ofxVec2f& target )
{
	bool l = target.x >= bounds.x;
	bool t = target.y >= bounds.y;
	bool r = target.x < bounds.x + bounds.width;
	bool b = target.y < bounds.y + bounds.height;
	
	return ( l && t && r && b );
}

void NawlzInfodelParticle :: constrainToBounds ( const ofxVec2f& target )		// target back to center of bounds.
{
	float cx = bounds.x + bounds.width  * 0.5;
	float cy = bounds.y + bounds.height * 0.5;
	
	float px = target.x - cx;
	float py = target.y - cy;
	
	ofxVec2f vec = ofxVec2f( px, py );
	float ang = vec.angle( ofxVec2f( 0, -1 ) );							// return an angle between -180 and 180.
	
	wanderTheta = ( ( ang + 180 ) / 360.0 ) * TWO_PI + PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
	wanderTheta *= -1;
}

void NawlzInfodelParticle :: draw ()
{
    float x, y;
    
    x = pos.x - widthHalf;
    y = pos.y - heightHalf;
    
    if( tex )
    {
        ofSetColor( 0xFFFFFF );
        tex->draw( x, y, width, height );
    }
    else
    {
        ofFill();
        ofSetColor( 0xFFFFFF );
        ofRect( (int)x, (int)y, width, height );
    }
}
