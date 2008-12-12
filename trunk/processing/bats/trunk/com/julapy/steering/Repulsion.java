package com.julapy.steering;

import java.awt.Rectangle;

import processing.core.PApplet;
import toxi.geom.Vec3D;

public class Repulsion 
{
	Particle[] ps;
	Rectangle[] rects;
	
	public float repulsionScale = 0;
	
	int width;
	int height;
	
	float maxRange = 2000;
	
	public Repulsion ( Particle[] ps, int width, int height )
	{
		this.ps		= ps;
		this.width	= width;
		this.height	= height;
	}
	
	public void update ( Rectangle[] rects )
	{
		this.rects = rects;
		
		int i, j, maxScale;
		float rs, s, d;
		Rectangle r;
		Vec3D rloc, sub, vel;
		Particle p;
		
		maxScale = width * height;
		
		for( i=0; i<rects.length; i++ )
		{
			r		= rects[ i ];
			rs		= r.width * r.height / (float)maxScale;
			
			rloc	= new Vec3D();
			rloc.x	= ( r.x + r.width * 0.5f) / width - 0.5f;
			rloc.y	= ( r.y + r.height * 0.5f) / height - 0.5f;
			rloc.z	= 0.2f;
			rloc.scaleSelf( 400 );
			
			for( j=0; j<ps.length; j++ )
			{
				p	= ps[ i ];
				sub	= rloc.sub( p.loc );
				d	= sub.magnitude();
				s	= getNegLinearScale( d, maxRange );

				vel	= sub.normalize();
				vel.scaleSelf( s * rs * repulsionScale );
				vel.scaleSelf( 20 );
				
//				PApplet.println( repulsionScale );
				
//				if( !vel.isZeroVector() )
//					PApplet.println( vel );
				
				p.vel.addSelf( vel );
			}
		}
	}
	
	public void update ( Rectangle r )
	{
		if( r == null )
			return;
		
		int i, maxScale;
		float rs, s, d;
		Vec3D rloc, sub, vel;
		Particle p;
		
		maxScale = width * height;

		rs		= r.width * r.height / (float)maxScale;
		
		rloc	= new Vec3D();
		rloc.x	= ( r.x + r.width * 0.5f) / width - 0.5f;
		rloc.y	= ( r.y + r.height * 0.5f) / height - 0.5f;
		rloc.z	= 0.3f;
		rloc.scaleSelf( 800 );
		
		for( i=0; i<ps.length; i++ )
		{
			
			p	= ps[ i ];
			sub	= rloc.sub( p.loc );
			d	= sub.magnitude();
			s	= getNegLinearScale( d, maxRange );
			
			vel	= sub.normalize();
			vel.scaleSelf( s * rs * repulsionScale );
			vel.scaleSelf( 50 );
			
//			if( !vel.isZeroVector() )
//				PApplet.println( vel );
			
			p.vel.addSelf( vel );
		}
	}
	
	private float getNegLinearScale ( float distance, float range )
	{
		if( distance < range )
		{
			return 1 - ( distance / range );
		}
		else
		{
			return 0;
		}
	}
	
}
