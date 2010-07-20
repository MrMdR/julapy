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

void ofxColorMap :: setup( ofImage *image )
{
	this->image = image;
}

int ofxColorMap :: getColor( const ofPoint& p )
{
	return getColor( p.x, p.y );
}

int ofxColorMap :: getColor( float x, float y )
{
	float cx, cy;
	
	cx = MAX( MIN( x, 1 ), 0 );
	cy = MAX( MIN( y, 1 ), 0 );
	
	unsigned char *pixels = image->getPixels();
	
	int imageDepth;
	imageDepth = image->bpp / 8;
	
	int w = (int)( ( image->width  - 1 ) * cx ) * imageDepth;
	int h = (int)( ( image->height - 1 ) * cy );
	int i = h * image->width * imageDepth + w;
	
	int r = pixels[ i + 0 ];
	int g = pixels[ i + 1 ];
	int b = pixels[ i + 2 ];
	
	int hex;
	
	hex = 
	( ( r & 0xff ) << 16 ) |
	( ( g & 0xff ) << 8  ) |
	( ( b & 0xff ) );
	
	return hex;
}
