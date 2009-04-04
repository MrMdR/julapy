package com.julapy.pargoliers;

import toxi.geom.Vec3D;
import toxi.math.MathUtils;

public class PargoliersParticle
{
	Vec3D loc;
	Vec3D vel;
	
	PargoliersParticle( Vec3D loc, Vec3D vel )
	{
		this.loc 		= loc;
		this.vel 		= vel;
	}
	
	public void update( Vec3D srcVec, Vec3D dstVec, float c )
	{
//		if( dstVec.x != 0 && dstVec.y != 0 )
//		{
//			vel.x = scaleValues( vel.x, -dstVec.x, 0.125f );
//			vel.y = scaleValues( vel.y, -dstVec.y, 0.125f );
//		}
//		
//		if( srcVec.x != 0 && srcVec.y != 0 )
//		{
//			vel.x = scaleValues( vel.x, srcVec.x, 0.15f );
//			vel.y = scaleValues( vel.y, srcVec.y, 0.15f );
//		}
//		
//		vel.x += MathUtils.random( -0.1f, 0.1f );
//		vel.y += MathUtils.random( -0.1f, 0.1f );
//		
//		double d		= vel.distanceTo( new Vec3D() );
//		double bwMix	= scaleValues( blackSpeed, whiteSpeed, c );
//		
//		vel.x += ( vel.x * ( 1/d * bwMix - 1 ) ) * 0.5f;
//		vel.y += ( vel.y * ( 1/d * bwMix - 1 ) ) * 0.5f;
//		
//		loc.addSelf( vel );
	}
}
