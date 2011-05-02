//
//  NawlzGardener.h
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "MSACore.h"
#include "NawlzGardenerParticle.h"

#define NAWLZ_GARDENER_APP_INDEX_0      0
#define NAWLZ_GARDENER_APP_INDEX_1      1
#define NAWLZ_GARDENER_APP_INDEX_2      2
#define NAWLZ_GARDENER_APP_INDEX_3      3

using namespace MSA;

class NawlzGardener : public ofBaseApp
{
    
public:
    
     NawlzGardener ( int appIndex = 0 );
    ~NawlzGardener ();
    
    void setup  ();
    void update ();
    void draw   ();
    
    void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createParticle01Texture    ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createParticle02Texture    ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    
    void addParticle0               ();
    void addParticle1               ();
    void addParticle2               ();
    void addParticle3               ();

    void killParticles              ();

    void updateParticles0           ();
    void updateParticles1           ();
    void updateParticles2           ();
    void updateParticles3           ();
    
    void drawParticles              ();
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    int             appIndex;
    
    ofTexture*      backgroundTexture;
    ofTexture*      particle01Texture;
    ofTexture*      particle02Texture;
    
    vector<NawlzGardenerParticle*>   particles;
};