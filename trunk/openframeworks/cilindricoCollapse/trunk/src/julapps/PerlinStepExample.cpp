/*
 *  PerlinStepExample.cpp
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 16/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "PerlinStepExample.h"

PerlinStepExample :: PerlinStepExample ()
{
	perlinRes			= 0.06;
	perlinBanding		= 0.1;
	perlinLowerBound	= 0.0;
	perlinUpperBound	= 1.0;
	
	ps.setup();
	ps.setSampleLength( 360 );
	ps.setResolution( perlinRes );
	ps.setBanding( 0.1 );				// banding rounds off values to every 0.1 between 0 - 1.
	
	gui.addSlider( "perlinRes",			perlinRes,			-0.5, 0.5 );
	gui.addSlider( "perlinBanding",		perlinBanding,		 0.0, 0.5 );
	gui.addSlider( "perlinUpperBound",	perlinUpperBound,	 0.0, 1.0 );
	gui.addSlider( "perlinLowerBound",	perlinLowerBound,	 0.0, 1.0 );
}

PerlinStepExample :: ~PerlinStepExample()
{

}

void PerlinStepExample :: update ()
{
	ps.setResolution( perlinRes );
	ps.setBanding( perlinBanding );
	ps.setBounds( perlinLowerBound, perlinUpperBound );
	ps.update();
}

void PerlinStepExample :: draw ()
{
	//-- draw bg.
	
	ofFill();
	ofSetColor( 50, 50, 50 );
	
	ofRect
	( 
		0,
		renderArea.height - 20 - renderArea.height * 0.5, 
		renderArea.width,
		renderArea.height * 0.5
	);
	
	//-- draw perlin steps.
	
	float w;
	int i;
	
	ofFill();
	ofSetColor( 128, 128, 128 );
	
	w = renderArea.width / (float)ps.length();
	
	for( i=0; i<ps.length(); i++ )
	{
		ofRect
		( 
			i * w,
			renderArea.height - 20, 
			w,
			-ps.at( i ) * renderArea.height * 0.5
		);
	}
	
	//-- draw upper / lower bounds.
	
	ofNoFill();
	ofSetLineWidth( 2.0 );
	ofSetColor( 255, 0, 255 );

	ofLine
	(
		0,
		renderArea.height - 20 - renderArea.height * 0.5 * perlinUpperBound,
		renderArea.width,
		renderArea.height - 20 - renderArea.height * 0.5 * perlinUpperBound
	);

	ofLine
	(
		0,
		renderArea.height - 20 - renderArea.height * 0.5 * perlinLowerBound,
		renderArea.width,
		renderArea.height - 20 - renderArea.height * 0.5 * perlinLowerBound
	);
	
	gui.draw();
}