/*
 *  QuatRibbon.h
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 28/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _H_QUAT_RIBBON
#define _H_QUAT_RIBBON

#include "Ribbon.h"
#include "ofxQuat.h"
#include "ofxMatrix4x4.h"

class QuatRibbon : public Ribbon
{

public :
	
	QuatRibbon();
	~QuatRibbon();
	
private :
	
	void calcLine();
	void rotateToSpherePoint ( float *m, float lat, float lon );
	void geodesicSlerp( float *m, float lat0, float lon0, float lat1, float lon1, float dt, bool ignoreDirection = false );
};

#endif