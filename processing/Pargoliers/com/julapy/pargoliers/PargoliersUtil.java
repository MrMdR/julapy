package com.julapy.pargoliers;

import toxi.geom.Vec3D;

public class PargoliersUtil 
{
	public static int resX	= 1;
	public static int resY	= 1;
	
	public static Vec3D getBrightnessVector( int x, int y, int width, int height, int pixels[], int reach )
	{
		Vec3D centrVec;
		Vec3D pixelVec;
		Vec3D dirVec;
		Vec3D colorVec;
		float c;
		int i, j;
		int l, r, t, b;
		
		l = x - reach * resX;
		r = x + reach * resX;
		t = y - reach * resY;
		b = y + reach * resY;
		
		centrVec = new Vec3D( x, y, 0 );
		colorVec = new Vec3D();
		
		for( i=l; i<=r; i+=resX )
		{
			for( j=t; j<=b; j+=resY )
			{
				if
				(
					i >= 0 &&
					i <= width - 1 && 
					j >= 0 &&
					j <= height - 1
				)
				{
					c 			= getColorValue( i, j, width, height, pixels );
					pixelVec	= new Vec3D( i, j, 0 );
					dirVec		= pixelVec.sub( centrVec );
					dirVec.normalize();
					dirVec.scaleSelf( c );
					colorVec.addSelf( dirVec );
				}
			}
		}
		
		if( !colorVec.isZeroVector() )	// if its non zero but really small, round it off to zero.
		{
			if( colorVec.magnitude() < 0.0001f )
			{
				colorVec = new Vec3D();
			}
		}
		
		colorVec.normalize();

		return colorVec;
	}
	
	public static float getColorValue ( int x, int y, int width, int height, int pixels[] )
	{
		int c;
		int r;
		int g;
		int b;
		float v;
		
		c = pixels[ x + y * width ];
		r = c >> 16 & 0xFF;
		g = c >> 8 & 0xFF;
		b = c & 0xFF;
		
		v = ( r + g + b ) / 765.0f;
		
//		System.out.println( "c :: " + c );
//		System.out.println( "r :: " + r );
//		System.out.println( "g :: " + g );
//		System.out.println( "b :: " + b );
//		System.out.println( "v :: " + v );
		
		return v;
	}

}
