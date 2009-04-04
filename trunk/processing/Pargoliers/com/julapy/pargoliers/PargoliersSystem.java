package com.julapy.pargoliers;

import java.util.ArrayList;

import processing.core.PApplet;
import toxi.geom.Vec3D;

public class PargoliersSystem
{
	private PApplet papp;
	
	private PargoliersParticle[]	particles;
	private int						partilcesMax = 20;
	
	private int[] srcPixels;
	private int[] dstPixels;
	
	private int scanArea = 4;
	
	PargoliersSystem ( PApplet papp )
	{
		this.papp = papp;
		
		init();
	}
	
	private void init ()
	{
		int i;
		
		particles = new PargoliersParticle[ partilcesMax ];
		
		for( i=0; i<partilcesMax; i++ )
		{
			particles[ i ] = new PargoliersParticle( new Vec3D(), new Vec3D() );
		}
	}

	public void update ( int[] srcPixels, int[] dstPixels )
	{
		this.srcPixels	= srcPixels;
		this.dstPixels	= dstPixels;
		
		PargoliersParticle p;
		Vec3D srcVec;
		Vec3D dstVec;
		float c;
		int i;
		
		for( i = 0; i<particles.length; i++ )
		{
			p = particles[ i ];
			
			srcVec = getBrightnessVector
			(
				(int)p.loc.x,
				(int)p.loc.y, 
				papp.width,
				papp.height,
				srcPixels,
				scanArea
			);
			
			dstVec = getBrightnessVector
			( 
				(int)p.loc.x,
				(int)p.loc.y, 
				papp.width,
				papp.height,
				dstPixels, 
				scanArea
			);
			
			c = getColorValue
			(
				(int)p.loc.x,
				(int)p.loc.y
			);
			
			p.update( srcVec, dstVec, c );
		}
	}
	
	private float getColorValue ( float x, float y )
	{
		int i;
		float val;
		
		i = srcPixels[ (int)x + (int)y * papp.width ];
		
		val = ( papp.red(i) + papp.blue(i) + papp.green(i) ) / 765.0f;
		
		return val;
	}

	private Vec3D getBrightnessVector( int x, int y, int w, int h, int pixels[], int area )
	{
		Vec3D centrVec;
		Vec3D pixelVec;
		Vec3D dirVec;
		Vec3D colorVec;
		float c;
		
		centrVec = new Vec3D( x, y, 0 );
		colorVec = new Vec3D();
		
		int rightBound	= PApplet.constrain( x + area, 0, w - 1 );
		int leftBound	= PApplet.constrain( x - area, 0, w - 1 );
		int bottomBound	= PApplet.constrain( y + area, 0, h - 1 );
		int topBound	= PApplet.constrain( y - area, 0, h - 1 );
		
		for(int i = leftBound; i <= rightBound; i++)
		{
			for(int j = topBound; j <= bottomBound; j++)
			{
				c 			= getColorValue( i, j );
				pixelVec	= new Vec3D( i, j, 0 );
				dirVec		= centrVec.sub( pixelVec );
				dirVec.scaleSelf( c );
				colorVec.addSelf( dirVec );
			}
		}

		return colorVec.normalize();
	}
}