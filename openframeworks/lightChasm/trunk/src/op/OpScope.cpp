/*
 *  OpScope.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 11/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpScope.h"

OpScope :: OpScope()
{

}

OpScope :: ~OpScope()
{

}

void OpScope :: init( int w, int h )
{
	width	= w;
	height	= h;
	
	videoPixels = new unsigned char[ width * height * 3 ];
	videoTexture.allocate( width, height, GL_RGB );
}

void OpScope :: update( unsigned char * pixels )
{
	int w2 = (int)( width * 0.5 );
	int h2 = (int)( height * 0.5 );
	
	for( int py=0; py<height; py++ )
	{
		for( int px=0; px<width; px++ )
		{
			int i;
			int j;
			
			i = ( px * 3 ) + ( py * width * 3 );
			
			if( py < h2 && px < w2 )
			{
				j = ( px * 3 ) + ( py * width * 3 );
			}
			
			if( py < h2 && px >= w2 )
			{
				j = ( ( width - px ) * 3 ) + ( py * width * 3 );
			}
			
			if( py >= h2 && px < w2 )
			{
				j = ( px * 3 ) + ( ( height - py ) * width * 3 );
			}
			
			if( py >= h2 && px >= w2 )
			{
				j = ( ( width - px ) * 3 ) + ( ( height - py ) * width * 3 );
			}
			
			memcpy( videoPixels + i, pixels + j, 3 * sizeof( unsigned char ) );
		}
	}
			
	videoTexture.loadData( videoPixels, width, height, GL_RGB );
}

void OpScope :: draw()
{
	videoTexture.draw( 0, 0 );
}