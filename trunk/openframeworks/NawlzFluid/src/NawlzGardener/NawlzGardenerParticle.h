//
//  NawlzGardenerParticle.h
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"

class NawlzGardenerParticle : public ofBaseApp
{
    
public:
    
     NawlzGardenerParticle();
    ~NawlzGardenerParticle();
    
    void setup              ();
    void setTexture         ( ofTexture* tex );
    void setBounds          ( const vector<ofPoint>& poly);
    void setBoundsCenter    ( const ofPoint& p );
    void setThetaClamp      ( float thetaCenter, float thetaClamp );
    
    void update ( float forceScale = 1.0 );
    void wander ();
    
    bool checkIsInsideBounds    ( const ofxVec2f& target );
    void constrainToBounds      ( const ofxVec2f& target );
    
    void draw   ();
    
    bool        bAlive;
    bool        bFadeOut;
    bool        bClampTheta;
    
    float       width;
    float       height;
    float       widthHalf;
    float       heightHalf;
    
    ofRectangle bounds;
    vector<ofPoint> boundsPoly;
    ofPoint     boundsCenter;
    
    ofxVec2f    pos;
    ofxVec2f    vel;
    ofxVec2f    fluidVel;
    
	ofxVec2f	wanderVel;
	ofxVec2f	wanderCircle;
	ofxVec2f	wanderCircleTarget;
	ofxVec2f	wanderCircleOffSet;
	float		wanderTheta;
    float       wanderThetaCenter;
    float       wanderThetaClamp;
	float		wanderRadius;			// Radius for our "wander circle"
	float		wanderDistance;			// Distance for our "wander circle"
	float		wanderChange;
	float		wanderEase;
	float		wanderEaseTarget;
	float		wanderMaxSpeed;
    
    float       sizeScale;
    float       alpha;
    float       alphaMax;
    
    float       rotation;
    float       rotationInc;
    
    ofTexture*  tex;
};