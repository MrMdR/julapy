/*
 *  ArcBar.h
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 12/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ARC_BAR_H_
#define _ARC_BAR_H_

#include "ofxVectorMath.h"
#include "ofxTrigLUT.h"
#include "PerlinStep.h"

class ArcBar
{

public :
	
	ArcBar();
	~ArcBar();
	
	void setup	();
	void setup	( PerlinStep *perlinStep );
	
	void createSolidModel ();
	void createWireframe  ();
	
	void update			  ();
	
	void renderSolidModel ();
	void renderWireframe  ();
	void render			  ();
	
	PerlinStep	*perlinStep;
	int			perlinSampleLegth;
	float		perlinResolution;
	float		perlinBanding;
	float		perlinLowerBounds;
	float		perlinUpperBounds;
	
	ofxVec3f loc;		// location.
	ofxVec3f vel;		// velocity.
	ofxVec3f rot;		// rotation.
	ofxVec3f dis;		// displacement.
	
	float rInc;
	
	float colour[ 4 ];
	float wfColour[ 4 ];
	
	int   id;
	float angle;
	int	  angleStep;
	float radius;
	float width;
	float height;
	float scale;
	float wireframePad;
	
	float normLength;
	
	bool drawQuads;
	bool drawNormals;
	bool drawAngles;
	
private :
	
	void init ();
	
};

#endif