//
//  NawlzJackInBoxPart.h
//  emptyExample
//
//  Created by lukasz karluk on 28/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxBounce.h"
#include "ofxVec2f.h"

class NawlzJackInBoxPart : public ofBaseApp
{
    
public:
    
     NawlzJackInBoxPart();
    ~NawlzJackInBoxPart();
    
    void setup  ( const ofPoint& p, float length, float angle );
    void update ();
    void draw   ( bool bDrawDebug = false );
    
    void updateForce    ( const ofPoint& v );
    void updateForce    ( int x, int y );
    
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
    
    ofTexture*  tex;
    ofPoint     texPos;
    ofPoint     texReg;
    
    bool        bJiggle;
    
    ofPoint     pStr;
    ofPoint     pEnd;
    ofPoint     pEndInit;
    float       length;
    float       angle;
    ofxBounce   bounce;
    
    ofxVec2f    pointDir;
    ofxVec2f    forcePoint;
    ofxVec2f    forceVec;
    float       forceScale;
    int         forceDir;
    float       forceMag;
};