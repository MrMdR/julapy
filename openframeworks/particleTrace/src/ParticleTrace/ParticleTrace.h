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
#include "ofxFBOTexture.h"
#include "ofImagePlus.h"
#include "ofxVec2f.h"
#include "PixelFlow.h"
#include "Particle.h"

class ParticleTrace : public ofBaseApp
{

public:
	
	 ParticleTrace();
	~ParticleTrace();
	
	void setup	();
	void update	();
	void draw	( bool bTiling = false );
	
	void loadImage			( string fileName );
	void addParticle		( float x, float y );
	void addRandomParticles ( int numOfParticles );
	void killAllParticles	();
	
	void updateParticles		();
	
	void drawSourceImage		();
	void drawTraceImage			();
	void drawParticles			( bool bDrawToFbo = false, bool bTiling = false );
	void drawSamples			();
	void drawSampleImage		( ofImage& img );
	void drawSampleVector		( const ofxVec2f& v );
	
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
	bool			bUseImageColour;
	bool			bEnableImageForce;
	bool			bEnableTraceForce;
	bool			bDrawParticleLines;
	bool			bDrawParticleStrip;
	bool			bDrawParticleHead;
	
	float			exportScale;
	float			lineWidth;

	ofxFBOTexture	fboTrace;
	ofxFBOTexture	fboParticles;
	
	ofImage			img;
	ofImagePlus		imgTrace;
	ofRectangle		imgRect;
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
};