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

#include "ofMain.h"
#include "trackball.h"
#include "ofxVec2f.h"

class ofxTrackball 
{

public :
	
	ofxTrackball();
	~ofxTrackball();
	
	void enableMouseEvents	();
	void disableMouseEvents ();
	void enableAppEvents	();
	void disableAppEvents	();
	
	void setCenter	( int x, int y );
	void mouseDown	( ofMouseEventArgs &e );
	void mouseUp	( ofMouseEventArgs &e );
	void mouseMoved	( ofMouseEventArgs &e );
	void update		( ofEventArgs &e );
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