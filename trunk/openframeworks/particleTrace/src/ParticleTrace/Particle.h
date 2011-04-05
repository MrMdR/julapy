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
#include "RibbonType2D.h"

#define PARTICLE_MAX_LENGTH		100000
//#define PARTICLE_MAX_LENGTH		100

class Particle
{

public:
	
	 Particle( PixelFlow* pfImage, PixelFlow* pfTrace );
	~Particle();

	void setup		();
	void update		();
	
	void wander					();
	bool checkIsInsideBounds	( const ofxVec2f& target );
	void constrainToBounds		( const ofxVec2f& target );
	
	void drawHead	();
	void drawLine	();
	void drawStrip	();
	void drawTrace	();
    void drawType   ();
	
	void setInitialPosition		( float x, float y );
	void setInitialVelocity		( float x, float y );
	void setPixelRange			( int x, int y );
	void setBounds				( const ofRectangle& rect );
    void setRibbonType          ( RibbonType2D* ribbonType, string ribbonCopy = "testing" );
	
	void addToStrip				();
	void addToLineVertexArray	( const ofPoint& p, const ofColor& c );
	void addToStripVertexArray	( const ofPoint& p1, const ofPoint& p2, const ofColor& c1, const ofColor& c2 );
    void addToRibbonType        ();
	
	void setLineColor			( const ofColor& c );
	void setLineAlpha			( float alpha );
	
	bool		bUseImageColour;
	bool		bUseImageForce;
	bool		bUseTraceForce;
	bool		bUseWanderForce;
	bool		bMarkAsTestParticle;
	bool		bVerbose;
	
	PixelFlow*	pfImage;
	PixelFlow*	pfTrace;
	int			pfSampleRangeX;
	int			pfSampleRangeY;
	
	ofRectangle	bounds;
	bool		bInsideBounds;
	
	int			pid;
	
	int			lifeCount;
	
	//--- vectors.
	
	ofxVec2f	posVec;
	ofxVec2f	posPrevVec;
	ofxVec2f	posLastAdded;
    ofxVec2f    posInImg;
	ofxVec2f	velVec;
	float		velLimit;
	float		velEase;
	ofPoint		stripPoint0;
	ofPoint		stripPoint1;
	
	ofxVec2f	imgVec;
	ofxVec2f	trcVec;
	
	ofxVec2f	wanderVel;
	ofxVec2f	wanderCircle;
	ofxVec2f	wanderCircleTarget;
	ofxVec2f	wanderCircleOffSet;
	float		wanderTheta;
	float		wanderRadius;			// Radius for our "wander circle"
	float		wanderDistance;			// Distance for our "wander circle"
	float		wanderChange;
	float		wanderEase;
	float		wanderEaseTarget;
	float		wanderMaxSpeed;
	
	float		imageVecScale;
	float		traceVecScale;
	float		wanderVecScale;
	
	//--- render.
	
	GLfloat*	line_ver_array;
	GLfloat*	line_col_array;
	int			line_ind_total;
	int			line_ind_max;
	
	GLfloat*	strip_ver_array;
	GLfloat*	strip_col_array;
	int			strip_ind_total;
	int			strip_ind_max;
	float		stripWidth;
	
	ofColor		currentColor;
	ofColor		lineColor;
	ofColor		headColor;
	float		colorEase;
	float		traceAlpha;
	float		minPosDist;
	
	int			size;
	int			sizeHalf;
    
    //--- ribbon type.
    
    RibbonType2D*   ribbonType;
    float           ribbonPositionX;
    string          ribbonCopy;
    int             ribbonCopyIndex;
    vector<Letter*> ribbonLetters;
};