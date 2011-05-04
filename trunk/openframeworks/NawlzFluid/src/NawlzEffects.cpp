//
//  NawlzEffects.cpp
//  emptyExample
//
//  Created by lukasz karluk on 6/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzEffects.h"

//--------------------------------------------------------------
void NawlzEffects :: setup()
{
	ofBackground(127,127,127);
	ofSetFrameRate( 30 );
    ofSetVerticalSync( true );
	
	stage = ofxFlashStage :: getInstance();			// ofxFlash setup.
	stage->setTouchMode( true );
	
	xfl = new ofxFlashXFL();
	xfl->loadFile( "Assets/DOMDocument.xml" );		// load XFL flash file.
	xfl->build();
	
	ofxFlashMovieClip* main;
	main = (ofxFlashMovieClip*)stage->root()->getChildByName( "main" );
    
    btnIDs.push_back( NAWLZ_FLUID_APP );
    btnIDs.push_back( NAWLZ_BLAH_APP );
    btnIDs.push_back( NAWLZ_SAND_APP );
    btnIDs.push_back( NAWLZ_FISH_BOWL_APP );
    btnIDs.push_back( NAWLZ_INFODEL_APP );
    btnIDs.push_back( NAWLZ_JACKINBOX_APP );
    btnIDs.push_back( NAWLZ_GARDENER_1_APP );
    btnIDs.push_back( NAWLZ_GARDENER_2_APP );
    btnIDs.push_back( NAWLZ_GARDENER_3_APP );
    btnIDs.push_back( NAWLZ_GARDENER_4_APP );
    btnIDs.push_back( NAWLZ_DANDELION_1_APP );
    btnIDs.push_back( NAWLZ_DANDELION_2_APP );
	
    int i = 0;
    while( true )
    {
        bool bUseBtn;
        bUseBtn = ( i <= btnIDs.size() - 1 );
        
        int btnAppID;
        btnAppID = btnIDs[ i ];
        
		string btnID = "";
		char btnIDChar[ 255 ];
		sprintf( btnIDChar, "btn%01d", i );
		btnID.insert( 0, btnIDChar );
		
		ofxFlashMovieClip* btnMc;
		btnMc = (ofxFlashMovieClip*)main->getChildByName( btnID );
		
		if( !btnMc )
			break;
        
        if( bUseBtn )
        {
            Btn* btn;
            btn = new Btn( btnMc, btnAppID, i + 1 );
            
            btns.push_back( btn );
        }
        else
        {
            btnMc->visible( false );
        }
        
        ++i;
    }

    btnSelected = btns[ 0 ];
    btnSelected->bSelected = true;
	
	nawlzApp	= NULL;
	appID		= -1;
	
//    loadApp( NAWLZ_FLUID_APP );
//    loadApp( NAWLZ_BLAH_APP );
//    loadApp( NAWLZ_SAND_APP );
//    loadApp( NAWLZ_FISH_BOWL_APP );
//    loadApp( NAWLZ_INFODEL_APP );
//    loadApp( NAWLZ_QUESTIONS_APP );
//    loadApp( NAWLZ_JACKINBOX_APP );
//    loadApp( NAWLZ_GARDENER_1_APP );
//    loadApp( NAWLZ_GARDENER_2_APP );
//    loadApp( NAWLZ_GARDENER_3_APP );
//    loadApp( NAWLZ_GARDENER_4_APP );
//    loadApp( NAWLZ_DANDELION_1_APP );
    loadApp( NAWLZ_DANDELION_2_APP );
}

void NawlzEffects :: loadApp ( int appID )
{
	if( this->appID == appID )
		return;
	
	this->appID = appID;
	
	if( nawlzApp )
	{
		delete nawlzApp;
		nawlzApp = NULL;
	}
	
	if( appID == NAWLZ_FLUID_APP )
	{
		NawlzFluid* nawlzFluid;
		nawlzFluid = new NawlzFluid();
		
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzFluid/whale_bg_1024x768.png" ), *img );
		nawlzFluid->createBackgroundTexture( img->pixels, img->width, img->height, img->glType, 0, 0 );
		delete img;
        img = NULL;
		
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzFluid/whale_477x223.png" ), *img );
		nawlzFluid->createImageTexture( img->pixels, img->width, img->height, img->glType, 358, 220 );
		delete img;
        img = NULL;
		
        nawlzFluid->useCircleMotion     = true;
		nawlzFluid->bDrawMeshGrid		= true;
		nawlzFluid->bDrawMeshQuads		= true;
		nawlzFluid->bDrawParticles		= true;
		
		nawlzFluid->setup();
		
		nawlzApp = nawlzFluid;
	}
	else if( appID == NAWLZ_BLAH_APP )
	{
		NawlzBlah* nawlzBlah;
		nawlzBlah = new NawlzBlah();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzBlah/blah_bg.png" ), *img );
		nawlzBlah->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzBlah/blah_particle_2.png" ), *img );
		nawlzBlah->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
		nawlzBlah->setup();
		
		nawlzApp = nawlzBlah;
	}
	else if( appID == NAWLZ_SAND_APP )
	{
		NawlzSand* nawlzSand;
		nawlzSand = new NawlzSand();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzSand/sand_bg.png" ), *img );
		nawlzSand->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
		nawlzSand->setup();
		
		nawlzApp = nawlzSand;
	}
	else if( appID == NAWLZ_FISH_BOWL_APP )
	{
		NawlzFishBowl* nawlzFishBowl;
		nawlzFishBowl = new NawlzFishBowl();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzFishBowl/fish_bowl_bg.png" ), *img );
		nawlzFishBowl->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzFishBowl/fish_bowl_trace.png" ), *img );
		nawlzFishBowl->createBowlTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
		nawlzFishBowl->setup();
		
		nawlzApp = nawlzFishBowl;
	}
    else if( appID == NAWLZ_INFODEL_APP )
    {
        NawlzInfodel* nawlzInfodel;
        nawlzInfodel = new NawlzInfodel();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_bg_clean.png" ), *img );
		nawlzInfodel->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_harley_0.png" ), *img );
		nawlzInfodel->createHarleyOneTexture( img->pixels, img->width, img->height, img->glType, 366, 311 );
		delete img;
        img = NULL;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_harley_1.png" ), *img );
		nawlzInfodel->createHarleyOneTexture( img->pixels, img->width, img->height, img->glType, 438, 366 );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_particle.png" ), *img );
		nawlzInfodel->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
		nawlzInfodel->setup();
		
		nawlzApp = nawlzInfodel;
    }
    else if( appID == NAWLZ_QUESTIONS_APP )
    {
        NawlzQuestions* nawlzQuestions;
        nawlzQuestions = new NawlzQuestions();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzQuestions/questions_bg.png" ), *img );
		nawlzQuestions->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzQuestions/questions_particle.png" ), *img );
		nawlzQuestions->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzQuestions->setup();
		
		nawlzApp = nawlzQuestions;
    }
    else if( appID == NAWLZ_JACKINBOX_APP )
    {
        NawlzJackInBox* nawlzJackInBox;
        nawlzJackInBox = new NawlzJackInBox();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_bg.png" ), *img );
		nawlzJackInBox->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
        //--- parts.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_body.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 629, 247 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_tongue.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 650, 534 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_lighter.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 759, 330 ), ofPoint( -5, -95 ), 120, 70, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_bomb_holder.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 768, 325 ), ofPoint( -8, -47 ), 70, 80, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_rocket.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 645, 326 ), ofPoint( -130, -47 ), 70, 270, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_arm_right.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 751, 425 ), ofPoint( -8, -95 ), 90, 75, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_arm_left.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 646, 426 ), ofPoint( -115, -73 ), 80, 270, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_bomb.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 723, 310 ), ofPoint( -6, -65 ), 105, 75, true );
        delete img;
        
        //--- ears right.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_right_3.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 725, 183 ), ofPoint( -4, -13 ), 50, 100, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_right_1.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 724, 180 ), ofPoint( -3, -33 ), 55, 70, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_right_2.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 726, 182 ), ofPoint( -5, -25 ), 70, 90, true );
        delete img;
        
        //--- brain.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_arrow.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 710, 157 ), ofPoint( -37, -63 ), 35, 0, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_brain.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 714, 138 ), ofPoint( -23, -71 ), 55, 10, true );
        delete img;
        
        //--- head.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_head_solo.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 667, 134 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_head_spine.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 695, 205 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
        //--- eyes left.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_left_L.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 698, 150 ), ofPoint( -69, -36 ), 60, -70, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_left_M.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 703, 147 ), ofPoint( -74, -66 ), 60, -45, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_left_R.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 708, 141 ), ofPoint( -30, -54 ), 45, -20, true );
        delete img;
        
        //--- eyes right.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_right_R.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 741, 150 ), ofPoint( -9, -55 ), 80, 60, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_right_M.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 739, 149 ), ofPoint( -7, -61 ), 55, 36, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_right_L.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 735, 147 ), ofPoint( -3, -64 ), 55, 13, true );
        delete img;
        
        //--- ears left.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_left_3.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 679, 177 ), ofPoint( -78, -5 ), 75, -125, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_left_2.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 678, 178 ), ofPoint( -69, -13 ), 45, -110, true );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_left_1.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 677, 174 ), ofPoint( -76, -44 ), 70, -75, true );
        delete img;
        
        //--- mouth.
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_mouth_base.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 699, 157 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_mouth_tongue.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 752, 211 ), ofPoint( -12, -7 ), 20, 135, true );
        delete img;
        
		nawlzJackInBox->setup();
		
		nawlzApp = nawlzJackInBox;
    }
    else if( appID == NAWLZ_GARDENER_1_APP )
    {
        NawlzGardener* nawlzGardener;
        nawlzGardener = new NawlzGardener( NAWLZ_GARDENER_APP_INDEX_0);
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_bg_01_1024x768.png" ), *img );
		nawlzGardener->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_01.png" ), *img );
		nawlzGardener->createParticle01Texture( img->pixels, img->width, img->height, img->glType );
		delete img;

		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_02.png" ), *img );
		nawlzGardener->createParticle02Texture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		nawlzGardener->setup();
		
		nawlzApp = nawlzGardener;
    }
    else if( appID == NAWLZ_GARDENER_2_APP )
    {
        NawlzGardener* nawlzGardener;
        nawlzGardener = new NawlzGardener( NAWLZ_GARDENER_APP_INDEX_1 );
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_bg_02_1024x768.png" ), *img );
		nawlzGardener->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_01.png" ), *img );
		nawlzGardener->createParticle01Texture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_02.png" ), *img );
		nawlzGardener->createParticle02Texture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzGardener->setup();
		
		nawlzApp = nawlzGardener;
    }
    else if( appID == NAWLZ_GARDENER_3_APP )
    {
        NawlzGardener* nawlzGardener;
        nawlzGardener = new NawlzGardener( NAWLZ_GARDENER_APP_INDEX_2 );
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_bg_03_1024x768.png" ), *img );
		nawlzGardener->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_01.png" ), *img );
		nawlzGardener->createParticle01Texture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_02.png" ), *img );
		nawlzGardener->createParticle02Texture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzGardener->setup();
		
		nawlzApp = nawlzGardener;
    }
    else if( appID == NAWLZ_GARDENER_4_APP )
    {
        NawlzGardener* nawlzGardener;
        nawlzGardener = new NawlzGardener( NAWLZ_GARDENER_APP_INDEX_3 );
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_bg_04_1024x768.png" ), *img );
		nawlzGardener->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_01.png" ), *img );
		nawlzGardener->createParticle01Texture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzGardener/gardener_particle_02.png" ), *img );
		nawlzGardener->createParticle02Texture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzGardener->setup();
		
		nawlzApp = nawlzGardener;
    }
    else if( appID == NAWLZ_DANDELION_1_APP )
    {
        NawlzDandelion* nawlzDanelion;
        nawlzDanelion = new NawlzDandelion();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_bg_01.png" ), *img );
		nawlzDanelion->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;

		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_fat_lady.png" ), *img );
		nawlzDanelion->createFatLadyTexture( img->pixels, img->width, img->height, img->glType, 251, 112, 330, 381 );
		delete img;

		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_text.png" ), *img );
		nawlzDanelion->createSpeachBubbleTexture( img->pixels, img->width, img->height, img->glType, 144, 136, 232, 134 );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_particle.png" ), *img );
		nawlzDanelion->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
        nawlzDanelion->setParticleCenter( 423, 334 );
        
		nawlzDanelion->setup();
		
		nawlzApp = nawlzDanelion;
    }
    else if( appID == NAWLZ_DANDELION_2_APP )
    {
        NawlzDandelion* nawlzDanelion;
        nawlzDanelion = new NawlzDandelion();
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_bg_02.png" ), *img );
		nawlzDanelion->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_fat_lady.png" ), *img );
		nawlzDanelion->createFatLadyTexture( img->pixels, img->width, img->height, img->glType, 390, 133, 255, 294 );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_text.png" ), *img );
		nawlzDanelion->createSpeachBubbleTexture( img->pixels, img->width, img->height, img->glType, 291, 149, 193, 111 );
		delete img;
        
		img = new NawlzImage();
		loadImage( ofToDataPath( "NawlzDandelion/dandelion_particle.png" ), *img );
		nawlzDanelion->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
        nawlzDanelion->setParticleCenter( 520, 292 );
        
		nawlzDanelion->setup();
		
		nawlzApp = nawlzDanelion;
    }
}

void NawlzEffects :: loadImage ( string path, NawlzImage& imageOut )
{
	ofImage image;
    image.loadImage( path );
	
	imageOut.width		= image.width;
	imageOut.height		= image.height;
	imageOut.pixelDepth	= image.bpp / 8;
	imageOut.glType		= GL_LUMINANCE;
	if( imageOut.pixelDepth == 3 )
		imageOut.glType	= GL_RGB;
	if( imageOut.pixelDepth == 4 )
		imageOut.glType	= GL_RGBA;
    
    int t = imageOut.width * imageOut.height * imageOut.pixelDepth;
    
	imageOut.pixels		= new unsigned char[ t ];
	
	memcpy( imageOut.pixels, image.getPixels(), t );
    
    image.clear();
}


//--------------------------------------------------------------
void NawlzEffects :: update()
{
	stage->update();
	
	for( int i=0; i<btns.size(); i++ )
	{
		Btn* btn;
		btn = btns[ i ];
		btn->update();
		
		if( btn->isPressed() && ( btn != btnSelected ) )
		{
            if( btnSelected )
            {
                btnSelected->bSelected = false;
            }
            
            btnSelected = btn;
            btnSelected->bSelected = true;
            
			loadApp( btnSelected->appID );
		}
	}
	
	if( nawlzApp )
	{
		nawlzApp->update();
	}
}

//--------------------------------------------------------------
void NawlzEffects :: draw()
{
	if( nawlzApp )
	{
		nawlzApp->draw();
	}
	
	stage->draw();
    
    for( int i=0; i<btns.size(); i++ )
        btns[ i ]->draw();
}

//--------------------------------------------------------------

void NawlzEffects :: keyPressed ( int key )
{
    nawlzApp->keyPressed( key );
}

void NawlzEffects :: keyReleased ( int key )
{
    nawlzApp->keyReleased( key );
}

void NawlzEffects :: mouseMoved ( int x, int y )
{
    nawlzApp->mouseMoved( x, y );
}

void NawlzEffects :: mouseDragged ( int x, int y, int button )
{
    nawlzApp->mouseMoved( x, y );
}

void NawlzEffects :: mousePressed ( int x, int y, int button )
{
    nawlzApp->mousePressed( x, y, button );
}

void NawlzEffects :: mouseReleased ( int x, int y, int button )
{
    nawlzApp->mouseReleased( x, y, button );
}

void NawlzEffects :: windowResized ( int w, int h )
{
    nawlzApp->windowResized( w, h );
}

