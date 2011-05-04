//
//  NawlzEffects.h
//  emptyExample
//
//  Created by lukasz karluk on 6/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#ifndef TARGET_OF_IPHONE

#include "ofxFlash.h"
#include "Btn.h"

#include "NawlzImage.h"
#include "NawlzFluid.h"
#include "NawlzBlah.h"
#include "NawlzSand.h"
#include "NawlzFishBowl.h"
#include "NawlzInfodel.h"
#include "NawlzQuestions.h"
#include "NawlzJackInBox.h"
#include "NawlzGardener.h"
#include "NawlzDandelion.h"

#define NAWLZ_FLUID_APP			0
#define NAWLZ_BLAH_APP			1
#define NAWLZ_SAND_APP			2
#define NAWLZ_FISH_BOWL_APP		3
#define NAWLZ_INFODEL_APP		4
#define NAWLZ_QUESTIONS_APP		5
#define NAWLZ_JACKINBOX_APP		6
#define NAWLZ_GARDENER_1_APP	7
#define NAWLZ_GARDENER_2_APP	8
#define NAWLZ_GARDENER_3_APP	9
#define NAWLZ_GARDENER_4_APP	10
#define NAWLZ_DANDELION_1_APP   11
#define NAWLZ_DANDELION_2_APP   12

class NawlzEffects : public ofBaseApp
{
    
public:    
    
     NawlzEffects () {};
    ~NawlzEffects () {};
	
	void setup	();
	void update	();
	void draw	();
    
	void loadApp		( int appID );
	void loadImage		( string path, NawlzImage& imageOut );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
	ofxFlashXFL*	xfl;
	ofxFlashStage*	stage;
	vector<Btn*>	btns;
    vector<int>     btnIDs;
    Btn*            btnSelected;
	ofBaseApp*		nawlzApp;
	int				appID;
    NawlzImage*     img;
};

#endif