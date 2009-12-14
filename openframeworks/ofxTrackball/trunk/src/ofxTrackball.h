/*
 *  ofxTrackball.h
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 14/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_TRACKBALL_H_
#define _OFX_TRACKBALL_H_

#include "trackball.h"
#include "ofxVec2f.h"

class ofxTrackball 
{

public :
	
	ofxTrackball();
	~ofxTrackball();
	
	void setCenter	( int x, int y );
	void mouseDown	( int x, int y );
	void mouseUp	( int x, int y );
	void mouseMoved	( int x, int y );
	void update		();
	void rotate		();
	void reset		();

	GLfloat		trackRot[ 4 ];
	GLfloat		worldRot[ 4 ];
	
	ofxVec2f	center;
	ofxVec2f	mouseCurr;
	ofxVec2f	mousePrev;
	ofxVec2f	mouseVel;
	bool		isMouseDown;
	
};

#endif