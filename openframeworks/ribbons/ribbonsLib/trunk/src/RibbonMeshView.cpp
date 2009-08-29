/*
 *  RibbonMeshView.cpp
 *  ribbonVBOTest
 *
 *  Created by lukasz karluk on 28/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonMeshView.h"

RibbonMeshView :: RibbonMeshView()
{
	useRibbonGLitch( false );
}

RibbonMeshView :: ~RibbonMeshView()
{
	//
}

void RibbonMeshView :: setup( Ribbon *r )
{
	ribbon = r;
	
	glGenBuffersARB( 2, &vbo[ 0 ] );
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ 0 ] );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, ribbon->trailLength * 3 * 2 * sizeof( float ), ribbon->trailVerticies, GL_STREAM_DRAW_ARB );
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ 1 ] );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, ribbon->trailLength * 4 * 2 * sizeof( float ), ribbon->trailColor, GL_STREAM_DRAW_ARB );
}

void RibbonMeshView :: draw()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ 0 ] );
	glBufferSubDataARB( GL_ARRAY_BUFFER_ARB, 0, ribbon->trailLength * 3 * 2 * sizeof( float ), ribbon->trailVerticies );
	glVertexPointer( 3, GL_FLOAT, 0, 0 );
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vbo[ 1 ] );
	glBufferSubDataARB( GL_ARRAY_BUFFER_ARB, 0, ribbon->trailLength * 4 * 2 * sizeof( float ), ribbon->trailColor );
	glColorPointer( 4, GL_FLOAT, 0, 0 );
	
	if( glitchItUp )
	{
		int  p1		= 0;
		int  p2		= 0;
		bool high	= false;
		bool drop	= false;
		
		for( int i=0; i<ribbon->trailLength; i++ )
		{
			int j = (int)( i / (float)( ribbon->trailLength - 1 ) * ( glitchArrayLength - 1 ) );
			
			if( glitchArray[ j ] == 1 && !high )
			{
				high	= true;
				p1		= j;
			}
			
			if( j == 0 )
				continue;
			
			if( ( glitchArray[ j ] == 0 && high ) || ( i == ribbon->trailLength - 1 && high ) )
			{
				drop	= true;
				high	= false;
				p2		= i;
			}
			
			if( drop )
			{
				int first = p1 * 2;
				int count = ( p2 - p1 ) * 2;
				glDrawArrays( GL_LINE_STRIP, first, count );
				
				drop = false;
			}
		}
	}
	else
	{
		glDrawArrays( GL_QUAD_STRIP, 0, ribbon->trailLength * 2 );
	}
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

void RibbonMeshView :: useRibbonGLitch( bool b )
{
	glitchItUp = b;
}

void RibbonMeshView :: setRibbonGlitch( int *floatArray, int arrayLength )
{
	glitchArray			= floatArray;
	glitchArrayLength	= arrayLength;
}