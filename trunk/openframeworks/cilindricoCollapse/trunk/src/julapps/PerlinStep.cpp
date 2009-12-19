/*
 *  PerlinStep.cpp
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 16/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "PerlinStep.h"

PerlinStep :: PerlinStep()
{
	perlinSamples = new float[ 1 ];		// initialise.
	
	reset();
	setSampleLength( 100 );
	setResolution( 1.0 );
	setBanding( 1.0 );
	setBounds( 0.0, 1.0 );
}

PerlinStep :: ~PerlinStep()
{
	delete [] perlinSamples;
}

void PerlinStep :: setup ()
{
	pn = new ofxPerlin();
}

void PerlinStep :: setup  ( ofxPerlin *perlin )
{
	pn = perlin;
}

void PerlinStep :: reset ()
{
	perlinVal	= 0;
	perlinSeed	= 0;
}

void PerlinStep :: update ()
{
	float perlinStart;
	perlinStart = perlinSeed - (int)( perlinSamplesLength * 0.5 ) * ABS( perlinRes );
	
	for( int i=0; i<perlinSamplesLength; i++ )
	{
		perlinVal	= pn->noiseuf( perlinStart += ABS( perlinRes ) );
		perlinVal	= (int)( perlinVal * perlinBandMult ) / (float)perlinBandMult;
		perlinVal	= ( perlinVal - perlinLowerBound ) * ( 1 / ( perlinUpperBound - perlinLowerBound ) );
		perlinVal	= MAX( perlinVal, 0 );
		perlinVal	= MIN( perlinVal, 1 );
		
		perlinSamples[ i ] = perlinVal;
	}
	
	perlinSeed -= perlinRes;
}

int PerlinStep :: length()
{
	return perlinSamplesLength;
}

float PerlinStep :: at( int i )
{
	return perlinSamples[ i ];
}

////////////////////////////////////////////////
//	SETTERS.
////////////////////////////////////////////////

void PerlinStep :: setSampleLength( int sampleLength )
{
	delete [] perlinSamples;
	perlinSamples = new float[ sampleLength ];
	
	perlinSamplesLength = sampleLength;
	
	for( int i=0; i<perlinSamplesLength; i++ )
	{
		perlinSamples[ i ] = 0;
	}
}

void PerlinStep :: setResolution( float resolution )
{
	perlinRes = resolution;
}

void PerlinStep :: setBanding( float banding )
{
	banding = MAX( MIN( banding, 1 ), 0 );
	
	if( banding == 0 )
		return;
	
	perlinBandMult = (int)( 1.0 / banding );
}

void PerlinStep :: setBounds ( float lowerBound, float upperBound )
{
	perlinLowerBound = lowerBound;
	perlinUpperBound = upperBound;
}