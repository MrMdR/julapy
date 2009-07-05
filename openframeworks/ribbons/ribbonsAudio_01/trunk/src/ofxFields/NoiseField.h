/*
 *  NoiseField.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxPerlin.h"
#include "ofxVectorMath.h"
#include "DataNormaliser.h"

class NoiseField
{
	
public:
	
	NoiseField();
	~NoiseField();

	void update();
	ofxVec3f NoiseField :: getNormalisedForce( float x, float y, float z );
	
	ofxPerlin	*noise;
	ofxVec3f	noiseCount;
	ofxVec3f	noiseSpeed;
	ofxVec3f	noiseScale;
	
	DataNormaliser	noiseNormX;
	DataNormaliser	noiseNormY;
	DataNormaliser	noiseNormZ;
};
