/*
 *  SphereUtil.h
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 21/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SPHERE_UTIL_H
#define SPHERE_UTIL_H

#include "ofMain.h"
#include "ofxVec3f.h"

ofxVec3f sphericalToCartesian( float lat, float lon, float radius );
ofxVec3f bernstein( float u, float *bezPoints );
void rotateToSphereSurface( float *m, float lat, float lon );
void rotateToSphereSurface( float *m, float x, float y, float z );

#endif