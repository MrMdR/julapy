//
//  NawlzQuestionsParticle.cpp
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzQuestionsParticle.h"

NawlzQuestionsParticle :: NawlzQuestionsParticle ()
{
    tex = NULL;

    float a, b, c, s;
    a = 1.5;
    b = 3.2;
    c = ofRandom( a, b );
    s = ( c - a ) / ( b - a );
    
	wanderTheta			= 0;
	wanderRadius		= 16.0;
	wanderDistance		= 60.0;
	wanderChange		= 0.25;
	wanderEase			= 0.2;
	wanderEaseTarget	= wanderEase;
    wanderMaxSpeed      = c;
    
    sizeScale           = 1.0;
    friction            = ofRandom( 0.5, 1.0 );
    
    rotation            = ofRandom( 0, TWO_PI );
    rotationInc         = ofRandom( TWO_PI * 0.01, TWO_PI * 0.02 );
    rotationInc         *= ( ofRandom( 0.0, 1.0 ) < 0.5 ) ? -1 : 1;
    
    width       = 2;
    height      = 2;
    widthHalf   = width  * 0.5;
    heightHalf  = height * 0.5;
}

NawlzQuestionsParticle :: ~NawlzQuestionsParticle ()
{
    //
}

void NawlzQuestionsParticle :: setup ()
{
    //
}

void NawlzQuestionsParticle :: setTexture ( ofTexture* tex )
{
    this->tex = tex;
    
    width       = tex->getWidth()  * sizeScale;
    height      = tex->getHeight() * sizeScale;
    widthHalf   = width  * 0.5;
    heightHalf  = height * 0.5;
}

void NawlzQuestionsParticle :: setBounds ( const vector<ofPoint>& poly )
{
    boundsPoly = poly;
}

void NawlzQuestionsParticle :: setBoundsCenter ( const ofPoint& p )
{
    boundsCenter = p;
}

void NawlzQuestionsParticle :: update ( float forceScale )
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
    
    rotation += rotationInc;
}

void NawlzQuestionsParticle :: wander()
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

bool NawlzQuestionsParticle :: checkIsInsideBounds ( const ofxVec2f& target )
{
    return ofInsidePoly( ofPoint( target.x, target.y ), boundsPoly );
    
//	bool l = target.x >= bounds.x;
//	bool t = target.y >= bounds.y;
//	bool r = target.x < bounds.x + bounds.width;
//	bool b = target.y < bounds.y + bounds.height;
//	
//	return ( l && t && r && b );
}

void NawlzQuestionsParticle :: constrainToBounds ( const ofxVec2f& target )		// target back to center of bounds.
{
	float cx = boundsCenter.x;
	float cy = boundsCenter.y;
	
	float px = target.x - cx;
	float py = target.y - cy;
	
	ofxVec2f vec = ofxVec2f( px, py );
	float ang = vec.angle( ofxVec2f( 0, -1 ) );							// return an angle between -180 and 180.
	
	wanderTheta = ( ( ang + 180 ) / 360.0 ) * TWO_PI + PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
	wanderTheta *= -1;
}

void NawlzQuestionsParticle :: draw ()
{
    float x, y;
    
    x = pos.x;
    y = pos.y;
    
    glPushMatrix();
    glTranslatef( x, y, 0 );
    glRotatef( rotation * RAD_TO_DEG, 0, 0, 1 );
    
    if( tex )
    {
        ofSetColor( 0xFFFFFF );
        tex->draw( -widthHalf, -heightHalf, width, height );
    }
    else
    {
        ofFill();
        ofSetColor( 0xFFFFFF );
        ofRect( -widthHalf, -heightHalf, width, height );
    }
    
    glPopMatrix();
}
