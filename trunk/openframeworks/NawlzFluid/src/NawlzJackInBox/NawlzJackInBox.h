//
//  NawlzJackInBox.h
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "NawlzJackInBoxPart.h"

#include "MSACore.h"
#include "MSAFluid.h"

using namespace MSA;

class NawlzJackInBox : public ofBaseApp
{
    
public:
    
     NawlzJackInBox ();
    ~NawlzJackInBox ();
    
    void setup  ();
    void update ();
    void draw   ();
    
    void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createBionicPart           ( unsigned char* pixels, int width, int height, int glType, const ofPoint& pos, const ofPoint& reg, float length, float rotation, bool jiggle = true );
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    ofTexture*  backgroundTexture;
    vector<NawlzJackInBoxPart*> parts;
    
    ofPoint     mousePrev;
    ofPoint     mouseCurr;
    ofxVec2f    mouseDir;
};