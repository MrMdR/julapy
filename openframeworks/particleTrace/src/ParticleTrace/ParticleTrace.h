/*
 *  ParticleTrace.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/03/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxResizeUtil.h"
#include "ofxFBOTexture.h"
#include "ofxVec2f.h"
#include "ofxCvColorImage.h"
#include "PixelFlow.h"
#include "Particle.h"
#include "RibbonType2D.h"

class ParticleTrace : public ofBaseApp
{

public:
	
	 ParticleTrace();
	~ParticleTrace();
	
	void setup	();
	void update	();
	void draw	( bool bTiling = false );
	
	void loadImage			( string fileName );
    void loadFont           ( string fileName );
    void loadCopy           ( string fileName );
    
	void addParticle		( float x, float y, bool bMarkAsTestParticle = false );
	void addRandomParticles ( int numOfParticles );
	void killAllParticles	();
	
	void updateParticles		();
	
	void drawSourceImage		();
	void drawTraceImage			();
	void drawParticles			( bool bDrawToFbo = false, bool bTiling = false );
    void drawRectOutline        ( const ofRectangle& rect );
	void drawSamples			();
	void drawSampleImage		( ofImage& img );
	void drawSampleVector		( const ofxVec2f& v );
    
    void saveTypeFBO    ();
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	bool			bImageLoaded;
	bool			bShowSourceImage;
	bool			bShowTraceImage;
	bool			bShowParticles;
	bool			bShowParticleLines;
	bool			bShowParticleStrip;
    bool            bShowParticleType;
	bool			bShowParticleHead;
	
	bool			bUseImageColour;
	bool			bUseImageForce;
	bool			bUseTraceForce;
	bool			bUseWanderForce;
	
	float			imageVecScale;
	float			traceVecScale;
	float			wanderVecScale;
	
	float			velLimit;
	float			velEase;
	
	float			exportScale;
	float			lineWidth;
	float			lineAlpha;
	float			traceAlpha;
	int				traceBlur;
	float			minPosDist;

	ofxFBOTexture	fboTrace;
	ofxFBOTexture	fboParticles;
    ofxFBOTexture   fboType;
    float           fboLargeScale;
    ofRectangle     fboLargeRect;
    
    ofRectangle     activeRect;
	
	ofImage			img;
	ofxCvColorImage	imgSrc;
	ofxCvColorImage	imgTrace;
	ofRectangle		imgRect;
    ofRectangle     imgRectCenter;
    ofRectangle     imgRectFit;
    ofRectangle     imgRectCrop;
    ofRectangle     imgRectCurrent;
	ofImage			sampleImage0;
	ofImage			sampleImage1;
	ofImage			sampleImage2;
	int				sampleRangeX;
	int				sampleRangeY;
	int				sampleW;
	int				sampleH;
	int				sampleGridW;
	int				sampleGridH;
	
	PixelFlow		pfImage;
	PixelFlow		pfTrace;
	ofxVec2f		pixelFlowVec;
	ofxVec2f		pixelFlowVecEased;
	
	int					pid;
	vector<Particle*>	particles;
	Particle*			testParticle;
    
    ofTrueTypeFont  font;
    float           fontSize;
    float           fontSizeAdjust;
    float           fontOffsetY;
    float           fontScale;
    float           fontKerning;
    
    RibbonType2D    ribbonType;
    float           ribbonPositionX;
    string          ribbonCopy;
};