/*
 *  ColorRect.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 10/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorRect.h"

ColorRect :: ColorRect ()
{
	rectVert = new float[ 8 ];
	rectVert[ 0 ] = 0;
	rectVert[ 1 ] = 0;
	rectVert[ 2 ] = ofGetWidth();
	rectVert[ 3 ] = 0;
	rectVert[ 4 ] = ofGetWidth();
	rectVert[ 5 ] = ofGetHeight();
	rectVert[ 6 ] = 0;
	rectVert[ 7 ] = ofGetHeight();
	
	rectCol = new float[ 12 ];
	rectCol[ 0 ]  = 1.0;
	rectCol[ 1 ]  = 0.0;
	rectCol[ 2 ]  = 0.0;
	rectCol[ 3 ]  = 1.0;
	rectCol[ 4 ]  = 0.0;
	rectCol[ 5 ]  = 0.0;
	rectCol[ 6 ]  = 0.0;
	rectCol[ 7 ]  = 0.0;
	rectCol[ 8 ]  = 1.0;
	rectCol[ 9 ]  = 0.0;
	rectCol[ 10 ] = 0.0;
	rectCol[ 11 ] = 1.0;
}

ColorRect :: ~ColorRect ()
{
	delete[] rectVert;
	delete[] rectCol;
}

void ColorRect :: setCornerColor( const ofColor& c, int cornerIndex )
{
	int i = ofClamp( cornerIndex, 0, 3 );
	
	rectCol[ i * 3 + 0 ] = c.r / 255.0;
	rectCol[ i * 3 + 1 ] = c.g / 255.0;
	rectCol[ i * 3 + 2 ] = c.b / 255.0;
}

void ColorRect :: draw ()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glVertexPointer( 2, GL_FLOAT, 0, rectVert );
	glColorPointer( 3, GL_FLOAT, 0, rectCol );
	
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}