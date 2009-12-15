/*
 *  ofxTrackball.h
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 14/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_TRACKBALL_H_
#define _OFX_TRACKBALL_H_

#include "ofMain.h"

class ofxTrackball 
{

public :
	
	ofxTrackball();
	~ofxTrackball();

	void setCenter	( int x, int y );
	void setRadius	( float radius );
	void setDamping	( float damping );
	
	void rotate		();
	void reset		();
	
private :
	
	void enableMouseEvents	();
	void disableMouseEvents ();
	void enableAppEvents	();
	void disableAppEvents	();
	
	void mouseDown	( ofMouseEventArgs &e );
	void mouseUp	( ofMouseEventArgs &e );
	void mouseMoved	( ofMouseEventArgs &e );
	void update		( ofEventArgs &e );

	void rollToTrackball		( );
	void addToRotationTrackball ( float *dA, float *A );
	void rotation2Quat			( float *A,  float *q );
	
	GLfloat		rotation[ 4 ];
	
	float		tolerance;
	float		radius;
	float		damping;
	
	ofPoint		center;
	ofPoint		mouseCurr;
	ofPoint		mousePrev;
	ofPoint		mouseVel;
	ofPoint		mouseNew;
	bool		isMouseDown;
	
};

#endif