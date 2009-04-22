/*
 *  NoiseField.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxPerlin.h"

class NoiseField
{
	
public:
	
	NoiseField();
	~NoiseField();

	void update();
	void get2DVectorAtPixel( int x, int y, float *nx, float *ny );
	
	ofxPerlin	*noise;
	float		noiseCount;
	float		noiseSpeed;
	float		noiseScale;
	float		noiseAngle;
	float		noiseX;
	float		noiseY;
	float		noiseZ;
	float		noiseDX;
	float		noiseDY;
	float		noiseDZ;
};
