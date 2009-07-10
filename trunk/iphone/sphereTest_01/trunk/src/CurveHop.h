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
	void playInStep();
	void playOutStep();
	void draw();
	
private :
	
	ofxVec3f bernstein( float u, float *bezPoints );
	
	float bezierPoints[ 12 ];
	float progressIn;
	float progressOut;
	float progressStep;
	int	  bezierDetail;
	bool  playedIn;
	bool  playedOut;
};