/*
 *  ColorTriangle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorTriangle.h"

ColorTriangle :: ColorTriangle ()
{

}

ColorTriangle :: ~ColorTriangle ()
{

}

void ColorTriangle :: init ()
{
	for( int i=0; i<3; i++ )
	{
		gl_points[ i * 2 + 0 ] = points[ i ].x;
		gl_points[ i * 2 + 1 ] = points[ i ].y;
		
		gl_colors[ i * 4 + 0 ] = colors[ i ].r / 255.0;
		gl_colors[ i * 4 + 1 ] = colors[ i ].g / 255.0;
		gl_colors[ i * 4 + 2 ] = colors[ i ].b / 255.0;
		gl_colors[ i * 4 + 3 ] = 1.0;
	}
}

void ColorTriangle :: draw ()
{
	drawFill();
	drawStroke();
}

void ColorTriangle :: drawFill ()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glVertexPointer( 2, GL_FLOAT, 0, &gl_points[ 0 ] );
	glColorPointer(  4, GL_FLOAT, 0, &gl_colors[ 0 ] );
	
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void ColorTriangle :: drawStroke ()
{
	bool bSmooth;
	bSmooth = true;
#ifdef TARGET_OF_IPHONE
	bSmooth = false;
#endif
	
	if( bSmooth )
	{
#ifndef TARGET_OPENGLES
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
#endif
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glVertexPointer( 2, GL_FLOAT, 0, &gl_points[ 0 ] );
	glColorPointer(  4, GL_FLOAT, 0, &gl_colors[ 0 ] );
	
	glDrawArrays( GL_LINE_LOOP, 0, 3 );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	
	if( bSmooth )
	{
#ifndef TARGET_OPENGLES
		glPopAttrib();
#endif
	}
}