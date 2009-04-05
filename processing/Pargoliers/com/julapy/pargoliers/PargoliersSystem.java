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
			
			srcVec = PargoliersUtil.getBrightnessVector
			(
				(int)p.loc.x,
				(int)p.loc.y, 
				papp.width,
				papp.height,
				srcPixels,
				scanArea
			);
			
			dstVec = PargoliersUtil.getBrightnessVector
			( 
				(int)p.loc.x,
				(int)p.loc.y, 
				papp.width,
				papp.height,
				dstPixels, 
				scanArea
			);
			
			c = PargoliersUtil.getColorValue
			(
				(int)p.loc.x,
				(int)p.loc.y,
				papp.width,
				papp.height,
				srcPixels
			);
			
			p.update( srcVec, dstVec );
		}
	}

}