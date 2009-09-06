/*
 *  ofxColorMap.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 27/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxColorMap.h"

ofxColorMap :: ofxColorMap()
{

}

ofxColorMap :: ~ofxColorMap()
{

}

void ofxColorMap :: loadImage( string fileName )
{
	image.loadImage( fileName );
}

void ofxColorMap :: getColorXY( float x, float y, int *r, int *g, int *b )
{
	float cx, cy;
	
	cx = MAX( MIN( x, 1 ), 0 );
	cy = MAX( MIN( y, 1 ), 0 );
	
	unsigned char *pixels = image.getPixels();
	
	int w = (int)( ( image.width  - 1 ) * cx ) * 3;
	int h = (int)( ( image.height - 1 ) * cy );
	int i = h * image.width * 3 + w;
	
	*r = pixels[ i + 0 ];
	*g = pixels[ i + 1 ];
	*b = pixels[ i + 2 ];
}
