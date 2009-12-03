/*
 *  ofxJulapyApp.cpp
 *  julapy_softbodies
 *
 *  Created by lukasz karluk on 22/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxJulapyApp.h"

void ofxJulapyApp :: setRenderArea( int x, int y, int w, int h )
{
	renderArea.x		= x;
	renderArea.y		= y;
	renderArea.width	= w;
	renderArea.height	= h;
}

void ofxJulapyApp :: setRenderArea( const ofRectangle& rect )
{
	renderArea.x		= rect.x;
	renderArea.y		= rect.y;
	renderArea.width	= rect.width;
	renderArea.height	= rect.height;
}

void ofxJulapyApp :: drawDebug()
{
	//
}
