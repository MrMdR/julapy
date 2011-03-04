/*
 *  Particle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/03/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"
#include "PixelFlow.h"

#define PARTICLE_MAX_LENGTH		10000

class Particle
{

public:
	
	 Particle( PixelFlow* pfImage, PixelFlow* pfTrace );
	~Particle();

	void setup		();
	void update		();
	
	void drawHead	();
	void drawLine	();
	void drawStrip	();
	void drawTrace	();
	
	void setInitialPosition		( float x, float y );
	void setInitialVelocity		( float x, float y );
	void setPixelRange			( int x, int y );
	
	void addToStrip				();
	void addToLineVertexArray	( const ofPoint& p, const ofColor& c );
	void addToStripVertexArray	( const ofPoint& p1, const ofPoint& p2, const ofColor& c1, const ofColor& c2 );
	
	bool		bEnableImageForce;
	bool		bEnableTraceForce;
	bool		bMarkAsTestParticle;
	bool		bUseImageColour;
	
	PixelFlow*	pfImage;
	PixelFlow*	pfTrace;
	int			pfSampleRangeX;
	int			pfSampleRangeY;
	
	int			pid;
	
	ofxVec2f	posVec;
	ofxVec2f	posPrevVec;
	ofxVec2f	velVec;
	ofPoint		stripPoint0;
	ofPoint		stripPoint1;

	GLfloat*	line_ver_array;
	GLfloat*	line_col_array;
	int			line_ind_total;
	
	GLfloat*	strip_ver_array;
	GLfloat*	strip_col_array;
	int			strip_ind_total;
	float		stripWidth;
	
	ofxVec2f	imgVec;
	ofxVec2f	trcVec;
	
	float		imgVecScale;
	float		trcVecScale;
	
	ofColor		currentColor;
	float		colorEase;
	
	int size;
	int sizeHalf;
	
	float		lineAlpha;
	
};