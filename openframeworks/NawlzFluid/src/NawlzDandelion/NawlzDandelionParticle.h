//
//  NawlzDandelionParticle.h
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"
#include "ofxVec3f.h"

class NawlzDandelionParticle : public ofBaseApp
{
    
public:
    
     NawlzDandelionParticle();
    ~NawlzDandelionParticle();
    
    void setup      ();
    void setTexture ( ofTexture* tex );

    void update     ( float forceScale = 1.0 );
    void draw       ();
    void drawBehind ();
    void drawFront  ();
    
    bool        bAlive;
    bool        bFadeOut;
    
    float       width;
    float       height;
    float       widthHalf;
    float       heightHalf;
    
    float       angle;
    float       radius;
    float       rotY;
    float       rotYInc;
    float       oscAng;
    float       oscAngInc;
    float       oscDist;
    
    ofxVec3f    pos;
    
    float       sizeScale;
    float       alpha;
    float       alphaMax;
    
    float       rotation;
    float       rotationInc;
    
    ofTexture*  tex;
};