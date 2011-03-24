//
//  NawlzQuestions.h
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#include "MSACore.h"
#include "MSAFluid.h"

#include "NawlzQuestionsParticle.h"

using namespace MSA;

class NawlzQuestions : public ofBaseApp
{
    
public:
    
     NawlzQuestions   ();
    ~NawlzQuestions   ();
    
    void setup  ();
    void update ();
    void draw   ();
    
    void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    void createParticleTexture      ( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );

    void addParticle                ( vector<NawlzQuestionsParticle*>& particles );
    void killParticles              ( vector<NawlzQuestionsParticle*>& particles );
    void updateParticles            ( vector<NawlzQuestionsParticle*>& particles );
    void drawParticles              ( vector<NawlzQuestionsParticle*>& particles );
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    ofTexture*      backgroundTexture;
    ofTexture*      particleTexture;
    
    ofRectangle     bounds;
    ofRectangle     boundsInner;
    vector<ofPoint> boundsPoints;
    ofPoint         boundsCenter;
    
    vector<NawlzQuestionsParticle*>   particles0;
};