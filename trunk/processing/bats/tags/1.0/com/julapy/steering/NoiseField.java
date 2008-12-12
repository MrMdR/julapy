package com.julapy.steering;

import toxi.geom.Vec3D;
import toxi.math.MathUtils;
import toxi.math.noise.PerlinNoise;

public class NoiseField 
{
	Particle[] ps;
	
	PerlinNoise	pn;
	float noiseXCount	= MathUtils.random( 100 );
	float noiseYCount	= MathUtils.random( 100 );
	float noiseZCount	= MathUtils.random( 100 );
	float noiseSpeed	= 0.01f;
	float noiseRadius	= 0;
	
	public float implodeVecScale	= 1.5f;
	public float noiseVecScale		= 1.0f;
	
	public NoiseField ( Particle[] ps, float noiseRadius )
	{
		this.ps 			= ps;
		this.noiseRadius	= noiseRadius;
		
		pn					= new PerlinNoise();
		pn.noiseDetail( 4, 0.95f );
	}
	
	public void update ()
	{
		Particle p;
		Vec3D implodeVec, noiseVec;
		float implodeRatio, noiseRatio, ratio;
		int i;
		
		for( i=0; i<ps.length; i++ )
		{
			p = ps[ i ];
			
			// implode / noise ratio.
//			ratio = ( noiseRadius - p.loc.magnitude() ) / noiseRadius;
//			if( ratio < 0 ) 
//			{
//				ratio = 0;
//			}
//			
//	        implodeRatio	= 1 - ratio;
//	        noiseRatio		= ratio;

			// implode / noise ratio.
			if( p.loc.magnitude() < noiseRadius )
			{
				implodeRatio	= 0;
				noiseRatio		= 1;
			}
			else
			{
				implodeRatio	= 1;
				noiseRatio		= 0;
			}
			
			// implode vector.
			implodeVec = p.loc.copy().normalize();
			implodeVec.scaleSelf( -implodeRatio );
			implodeVec.scaleSelf( implodeVecScale );
			
			// noise vector.
			float noiseXVal = pn.noise( p.loc.x * 0.005f, noiseXCount ) - 0.9f;
			float noiseYVal = pn.noise( p.loc.y * 0.005f, noiseYCount ) - 0.9f;
			float noiseZVal = pn.noise( p.loc.z * 0.005f, noiseZCount ) * 0.005f;
			
			noiseXCount += noiseSpeed;
			noiseYCount += noiseSpeed;
			noiseZCount += noiseSpeed;
			
			noiseVec = new Vec3D( noiseXVal, noiseYVal, noiseZVal); 
			noiseVec.scaleSelf( noiseRatio );
			noiseVec.scaleSelf( noiseVecScale );
			
			p.vel.addSelf( implodeVec );
			p.vel.addSelf( noiseVec );
		}
	}
}
