/*
 *  ofxScreen.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxScreen.h"

ofxScreen :: ofxScreen()
{
	set( 0, 0 );
}

ofxScreen :: ofxScreen( int w, int h )
{
	set( w, h );
}

ofxScreen :: ~ofxScreen ()
{

}

void ofxScreen :: set ( int w, int h )
{
	screenWidth		= w;
	screenHeight	= h;
	screenMax		= MAX( w, h );
	screenMin		= MIN( w, h );
	screenArea		= w * h;
	screenDiagonal	= sqrt( w * w + h * h );
}