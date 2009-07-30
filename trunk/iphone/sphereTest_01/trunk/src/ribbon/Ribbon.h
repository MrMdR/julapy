/*
 *  Ribbon.h
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 29/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _H_RIBBON
#define _H_RIBBON

#include "ofMain.h"
#include "SphereUtil.h"
#include "ofxVec3f.h"

class Ribbon
{
	
public :
	
	Ribbon();
	~Ribbon();
	
	void set( float *points );
	void reset();
	void setDetail( int value );
	void setProgressIn( float value );
	void setProgressOut( float value );
	void setProgressStep( float value );
	void setCurveWidth( float value );
	void setCurveHeight( float value );
	void playInStep();
	void playOutStep();
	bool isPlayedIn();
	bool isPlayedOut();

	void drawLine();
	void drawFill();
	
//private :
	
	virtual void calcLine();
	void drawLineStrip( GLfloat *vertex, GLfloat *color, int first, int count );
	void drawTriangleStrip( GLfloat *vertex, GLfloat *normal, GLfloat *color, int first, int count );
	
	float points[ 4 ];
	GLfloat *lineStrip;
	GLfloat	*lineColor;
	GLfloat *triangleStrip;
	GLfloat *triangleStripNormal;
	GLfloat *triangleStripColor;
	
	float progressIn;
	float progressOut;
	float progressStep;
	bool  playedIn;
	bool  playedOut;
	int   detail;
	float curveWidth;
	float curveHeight;

};

#endif