/*
 *  PixelFlow.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 6/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "PixelFlow.h"

PixelFlow :: PixelFlow()
{
	//
}

PixelFlow :: ~PixelFlow()
{
	//
}

void PixelFlow :: setPixels ( unsigned char* pix, int width, int height, int type )
{
	pixels		= pix;
	pixelsW		= width;
	pixelsH		= height;
	pixelsType	= type;
	
	pixelsBpp = 1;
	if( type == OF_IMAGE_COLOR )
		pixelsBpp = 3;
	if( type == OF_IMAGE_COLOR_ALPHA )
		pixelsBpp = 4;
}

ofxVec2f PixelFlow :: getVectorAt( const ofPoint& point, int rangeX, int rangeY )
{
	int w = rangeX * 2 + 1;
	int h = rangeY * 2 + 1;
	
	int topLeftX = point.x - rangeX;
	int topLeftY = point.y - rangeY;
	
	int px = 0;
	int py = 0;
	int i  = 0;
	int c  = 0;
	
	bool b1;
	bool b2;
	
	ofxVec2f p;
	ofxVec2f v;
	
	for( int y=0; y<h; y++ )
	{
		for( int x=0; x<w; x++ )
		{
			px = topLeftX + x;
			py = topLeftY + y;
			
			b1 = ( px < 0 || px > pixelsW - 1 );
			b2 = ( py < 0 || py > pixelsH - 1 );
			
			if( b1 || b2 )
			{
				c = 0;
				continue;
			}
			else
			{
				i = ( py * pixelsW + px ) * pixelsBpp;
				
				c = 0;
				for( int j=0; j<pixelsBpp; j++ )
				{
					c += pixels[ i + j ];
				}

				p.x = x - rangeX;
				p.y = y - rangeY;
				p.normalize();									// direction vector.
				
				p *= c / (float)( 255.0 * pixelsBpp );			// weighed by colour strength.
				
				v += p;
			}
		}
	}
	
	v /= ( ( w * h ) - 1 );		// minus one as the center point doesn't count.
	
	return v;
}

ofColor PixelFlow :: getColourAt( const ofPoint& point )
{
	ofColor c;
	
	int px = point.x;
	int py = point.y;

	bool b1;
	bool b2;
	
	b1 = ( px < 0 || px > pixelsW - 1 );
	b2 = ( py < 0 || py > pixelsH - 1 );
	
	if( b1 || b2 )
	{
		c.r = 0;
		c.g = 0;
		c.b = 0;
		c.a = 255;
		return c;
	}

	int i = ( py * pixelsW + px ) * pixelsBpp;
	
	if( pixelsType == OF_IMAGE_GRAYSCALE )
	{
		c.r = pixels[ i ];
		c.g = pixels[ i ];
		c.b = pixels[ i ];
		c.a = 255;
		return c;
	}
	else if( pixelsType == OF_IMAGE_COLOR )
	{
		c.r = pixels[ i + 0 ];
		c.g = pixels[ i + 1 ];
		c.b = pixels[ i + 2 ];
		c.a = 255;
		return c;
	}
	else if( pixelsType == OF_IMAGE_COLOR_ALPHA )
	{
		c.r = pixels[ i + 0 ];
		c.g = pixels[ i + 1 ];
		c.b = pixels[ i + 2 ];
		c.a = pixels[ i + 3 ];
		return c;
	}
	
	return c;
}

void PixelFlow :: getPixelsAt( const ofPoint& point, int rangeX, int rangeY, ofImage& img )
{
	int w = rangeX * 2 + 1;
	int h = rangeY * 2 + 1;
	
	bool b1 = img.width  != w;
	bool b2 = img.height != h;
	bool b3 = img.type   != pixelsType;
	
	if( b1 || b2 || b3 )
	{
		img.clear();
		img.allocate( w, h, pixelsType );
	}
	
	unsigned char* pixelsOut = img.getPixels();
	
	int topLeftX = point.x - rangeX;
	int topLeftY = point.y - rangeY;
	
	int px = 0;
	int py = 0;
	int p1 = 0;
	int p2 = 0;
	
	for( int y=0; y<h; y++ )
	{
		for( int x=0; x<w; x++ )
		{
			px = topLeftX + x;
			py = topLeftY + y;
			
			p1 = ( py * pixelsW + px ) * pixelsBpp;		// image pixel index.
			p2 = ( y * w + x ) * pixelsBpp;				// sample pixel index.
			
			bool b1 = ( px < 0 || px > pixelsW - 1 );
			bool b2 = ( py < 0 || py > pixelsH - 1 );
			bool b3 = ( b1 || b2 );
			
			for( int i=0; i<pixelsBpp; i++ )
			{
				pixelsOut[ p2 + i ] = b3 ? 0 : pixels[ p1 + i ];
			}
		}
	}
}