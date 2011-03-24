//
//  NawlzInfodelParticle.h
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"

class NawlzInfodelParticle : public ofBaseApp
{
    
public:
    
     NawlzInfodelParticle();
    ~NawlzInfodelParticle();
    
    void setup      ();
    void setTexture ( ofTexture* tex );
    
    void update ( float forceScale = 1.0 );
    void wander ();
    
    bool checkIsInsideBounds    ( const ofxVec2f& target );
    void constrainToBounds      ( const ofxVec2f& target );
    
    void draw   ();
    
    float       width;
    float       height;
    float       widthHalf;
    float       heightHalf;
    
    ofRectangle bounds;
    
    ofxVec2f    pos;
    ofxVec2f    vel;
    ofxVec2f    fluidVel;
    
	ofxVec2f	wanderVel;
	ofxVec2f	wanderCircle;
	ofxVec2f	wanderCircleTarget;
	ofxVec2f	wanderCircleOffSet;
	float		wanderTheta;
	float		wanderRadius;			// Radius for our "wander circle"
	float		wanderDistance;			// Distance for our "wander circle"
	float		wanderChange;
	float		wanderEase;
	float		wanderEaseTarget;
	float		wanderMaxSpeed;
    
    float       sizeScale;
    
    ofTexture*  tex;
    
    float friction;
};