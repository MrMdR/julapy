/*
 *  PerlinStep.h
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 16/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PERLIN_STEP_H_
#define _PERLIN_STEP_H_

#include "ofMain.h"
#include "ofxPerlin.h"

class PerlinStep
{

public :
	
	PerlinStep();
	~PerlinStep();
	
	void	setup  ();
	void	setup  ( ofxPerlin *perlin );
	void	reset  ();
	void	update ();
	int		length ();
	float	at     ( int i );
	
	void	setSampleLength	( int   sampleLength );
	void	setResolution	( float resolution );
	void	setBanding		( float banding );
	void	setBounds		( float lowerBound, float upperBound );
	
private :
	
	ofxPerlin	*pn;
	float		*perlinSamples;
	int			perlinSamplesLength;
	float		perlinVal;
	float		perlinSeed;
	float		perlinRes;
	int			perlinBandMult;
	float		perlinLowerBound;
	float		perlinUpperBound;
};

#endif