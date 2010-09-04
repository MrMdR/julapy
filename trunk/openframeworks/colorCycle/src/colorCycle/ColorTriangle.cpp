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
	for( int i=0; i<3; i++ )
	{
		gl_points[ i * 2 + 0 ] = 0;
		gl_points[ i * 2 + 1 ] = 0;
		
		gl_colors[ i * 4 + 0 ] = 0;
		gl_colors[ i * 4 + 1 ] = 0;
		gl_colors[ i * 4 + 2 ] = 0;
		gl_colors[ i * 4 + 3 ] = 0;
		
		points[ i ].x	= 0;
		points[ i ].y	= 0;
		points[ i ].z	= 0;
		
		colors[ i ].r	= 0;
		colors[ i ].g	= 0;
		colors[ i ].b	= 0;
		colors[ i ].a	= 0;
		
		indexs[ i ]		= 0;
	}
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

bool ColorTriangle :: isPointInsideTriangle( const ofPoint& p )
{
	int counter;
	counter = 0;
	
	ofPoint p1, p2;
	
	p1 = points[ 0 ];
	
	for( int i=1; i<=3; i++ )
	{
		p2 = points[ i % 3 ];
		
		if( p.y > MIN( p1.y, p2.y ) )
		{
			if( p.y <= MAX( p1.y, p2.y ) )
			{
				if( p.x <= MAX( p1.x, p2.x ) )
				{
                    if( p1.y != p2.y )
					{
                        double xinters;
						xinters = ( p.y - p1.y ) * ( p2.x - p1.x ) / ( p2.y - p1.y ) + p1.x;
						
						if( p1.x == p2.x || p.x <= xinters )
						{
							counter++;
						}
					}
				}
			}
		}
        p1 = p2;
    }
	
	if (counter % 2 == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

ofPoint ColorTriangle :: getNearestTrianglePoint ( float x, float y )
{
	int index	= 0;
	int dist	= 99999;
	
	for( int i=0; i<3; i++ )
	{
		float d = ofDist( x, y, points[ i ].x, points[ i ].y );
		
		if( d < dist )
		{
			dist	= d;
			index	= i;
		}
	}
	
	ofPoint p;
	p.x = points[ index ].x;
	p.y = points[ index ].y;
	
	return p;
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