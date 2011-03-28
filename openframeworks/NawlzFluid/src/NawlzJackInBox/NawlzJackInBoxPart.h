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
    void draw   ();
    
    void updateForce    ( const ofPoint& v );
    void updateForce    ( int x, int y );
    
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
    
    ofPoint     pStr;
    ofPoint     pEnd;
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