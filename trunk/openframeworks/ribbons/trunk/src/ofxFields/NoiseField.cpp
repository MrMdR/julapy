/*
 *  NoiseField.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "NoiseField.h"

NoiseField :: NoiseField()
{
	noise = new ofxPerlin();
	
	float noiseCountSpread = 100;
	float noiseSpeedSpread = 0.001;
	float noiseScaleSpread = 0.003;
	
	noiseCount.x = ofRandom( -1, 1 ) * noiseCountSpread;
	noiseCount.y = ofRandom( -1, 1 ) * noiseCountSpread;
	noiseCount.z = ofRandom( -1, 1 ) * noiseCountSpread;
	
	noiseSpeed.x = ofRandom( 1, 1 ) * noiseSpeedSpread;
	noiseSpeed.y = ofRandom( 1, 1 ) * noiseSpeedSpread;
	noiseSpeed.z = ofRandom( 1, 1 ) * noiseSpeedSpread;
	
	noiseScale.x = ofRandom( 1, 1 ) * noiseScaleSpread;
	noiseScale.y = ofRandom( 1, 1 ) * noiseScaleSpread;
	noiseScale.z = ofRandom( 1, 1 ) * noiseScaleSpread;
}

NoiseField :: ~NoiseField()
{
	delete noise;
}

void NoiseField :: update()
{
	noiseCount += noiseSpeed;
}

ofxVec3f NoiseField :: getNormalisedForce( float x, float y, float z )
{
	ofxVec3f vel;
	ofxVec3f rotX;
	ofxVec3f rotZ;
	
	rotX.set( 0, 1, 0 );
	rotZ.set( 0, 1, 0 );
	
	float noiseX;
	float noiseY;
	float noiseZ;
	
	noiseX = noise->noise( x * noiseScale.x, y * noiseScale.y, noiseCount.x );
	noiseY = noise->noise( x * noiseScale.x, z * noiseScale.z, noiseCount.y );
	noiseZ = noise->noise( y * noiseScale.y, z * noiseScale.z, noiseCount.z );
	
	float normX = noiseNormX.getNormalisedValue( noiseX );
	float normY = noiseNormY.getNormalisedValue( noiseY );
	float normZ = noiseNormZ.getNormalisedValue( noiseZ );
	
	rotX.rotate( normX * 360, ofxVec3f( 1, 0, 0 ) );
	rotZ.rotate( normZ * 360, ofxVec3f( 0, 0, 1 ) );
	
	vel = rotX + rotZ;
	vel.normalize();
	
	return vel;
}
