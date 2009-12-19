/*
 *  PerlinStepExample.h
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 16/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PERLIN_STEP_EXAMPLE_H_
#define _PERLIN_STEP_EXAMPLE_H_

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxSimpleGuiToo.h"
#include "PerlinStep.h"


class PerlinStepExample : public ofxJulapyApp
{

public :
	
	PerlinStepExample();
	~PerlinStepExample();
	
	void update();
	void draw();

	PerlinStep			ps;
	ofxSimpleGuiToo		gui;
	
	float		perlinRes;
	float		perlinBanding;
	float		perlinLowerBound;
	float		perlinUpperBound;
};

#endif