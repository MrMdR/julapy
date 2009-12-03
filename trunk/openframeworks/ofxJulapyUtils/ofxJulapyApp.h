/*
 *  ofxJulapyApp.h
 *  julapy_softbodies
 *
 *  Created by lukasz karluk on 22/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_JULAPY_APP_H_
#define _OFX_JULAPY_APP_H_

#include "ofMain.h"

class ofxJulapyApp : public ofBaseApp
{

public :

	virtual void drawDebug();
	virtual void setRenderArea( int x, int y, int w, int h );
	virtual void setRenderArea( const ofRectangle& rect );

	ofRectangle renderArea;
};

#endif