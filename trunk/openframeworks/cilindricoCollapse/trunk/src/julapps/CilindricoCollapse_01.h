/*
 *  CilindricoCollapse_01.h
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxJulapyApp.h"
#include "ofxTrigLUT.h"

#include "ArcBar.h"

class CilindricoCollapse_01 : public ofxJulapyApp
{

public :
	
	CilindricoCollapse_01 ();
	~CilindricoCollapse_01 ();
	
	void setup();
	void reset();
	void update();
	void draw();
	void drawDebug();
	
	void keyPressed( int key );

	vector<ArcBar> arcBars;
};