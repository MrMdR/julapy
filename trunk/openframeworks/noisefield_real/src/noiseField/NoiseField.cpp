/*
 *  NoiseField.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "NoiseField.h"

NoiseField :: NoiseField()
{
	noise = new ofxPerlin();
	
	noiseCount	= 0.0f;
	noiseSpeed	= 0.01f;
	noiseScale	= 0.005f;
	noiseAngle	= 720.0f;
	
	noiseX	= 0;
	noiseY	= 0;
	noiseZ	= 0;
	
	noiseDX = 0;
	noiseDY = 0;
	noiseDZ = 0;
}

NoiseField :: ~NoiseField()
{
	delete noise;
}

void NoiseField :: update()
{
	noiseCount += noiseSpeed;
}

void NoiseField :: get2DVectorAtPixel( int x, int y, float *nx, float *ny )
{
	float noiseVal;
	
	noiseVal = noise->noise( x * noiseScale, y * noiseScale, noiseCount );
	noiseVal *= noiseAngle;
	
	*nx = cos( DEG_TO_RAD * noiseVal );
	*ny = sin( DEG_TO_RAD * noiseVal );
}
