/*
 *  CurveHop.h
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 10/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxVectorMath.h"

class CurveHop
{
public :
	
	CurveHop();
	~CurveHop();
	
	void set( float *bezPoints );
	void reset();
	void setProgressIn( float value );
	void setProgressOut( float value );
	void setProgressStep( float value );
	void setCurveWidth( float value );
	void playInStep();
	void playOutStep();
	bool isPlayedIn();
	bool isPlayedOut();
	void draw();
	
private :
	
	ofxVec3f bernstein( float u, float *bezPoints );
	void drawCurveLineStrip( GLfloat *vertex, GLfloat *color, int length );
	void drawCurveTriangleStrip( GLfloat *vertex, GLfloat *color, int length );
	
	float bezierPoints[ 12 ];
	float progressIn;
	float progressOut;
	float progressStep;
	int	  bezierDetail;
	bool  playedIn;
	bool  playedOut;
	float curveWidth;
};