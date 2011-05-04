//
//  NawlzDandelion.h
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "MSACore.h"
#include "NawlzDandelionParticle.h"

using namespace MSA;

class NawlzDandelion : public ofBaseApp
{
    
public:
    
     NawlzDandelion ();
    ~NawlzDandelion ();
    
    void setup  ();
    void update ();
    void draw   ();
    
    void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createFatLadyTexture       ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0, int w=0, int h=0 );
    void createSpeachBubbleTexture  ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0, int w=0, int h=0 );
    void createParticleTexture      ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void setParticleCenter          ( float x, float y );
    
    void addParticle                ();
    void killParticles              ();
    void drawParticlesBehind        ();
    void drawParticlesFront         ();
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    int             appIndex;
    
    ofTexture*      backgroundTexture;
    ofTexture*      fatLadyTexture;
    ofTexture*      speachBubbleTexture;
    ofTexture*      particleTexture;
    
    ofRectangle     fatLadyTextureRect;
    ofRectangle     speachBubbleTextureRect;
    
    ofPoint         particleCenter;
    
    vector<NawlzDandelionParticle*>   particles;
};