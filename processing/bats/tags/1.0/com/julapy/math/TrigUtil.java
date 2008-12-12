package com.julapy.math;

public class TrigUtil 
{
	public static final float PI			= 3.14159265358979323846f;
	public static final float HALF_PI		= PI / 2;
	public static final float THIRD_PI		= PI / 3;
	public static final float QUARTER_PI	= PI / 4;
	public static final float TWO_PI		= PI * 2.0f;
	public static final float RADTODEG		= 180 / PI;		
	public static final float DEGTORAD		= PI / 180;
	
	public static float[] getPointOnCircle( float angleDegrees, float circleRadius )
	{
		float x;
		float y;
		float p[];
		
		x = circleRadius * (float)Math.cos( angleDegrees * DEGTORAD );
		y = circleRadius * (float)Math.sin( angleDegrees * DEGTORAD );

		p		= new float[2];
		p[ 0 ]	= x;
		p[ 1 ]	= y;
		
		return p;
	}
	
	public static float[] sphericalToCartesian ( float theta, float phi, float radius )
	{
		float p[];
		
		p = new float[ 3 ];
		
		p[ 0 ] = (float)Math.cos( theta ) * (float)Math.sin( phi ) * radius;
		p[ 1 ] = (float)Math.sin( theta ) * (float)Math.sin( phi ) * radius;
		p[ 2 ] = (float)Math.cos( phi ) * radius;
		
		return  p;
	}
	
	public static float[] cartesianToSpherical ( float x, float y, float z )
	{
		float theta, phi, radius;
		float p[];
		
		p = new float[ 3 ];
		
		radius	= (float)Math.sqrt( x * x + y * y + z * z );
		theta	= (float)Math.atan2( y, x );
		phi		= (float)Math.atan2( Math.sqrt( x * x + y * y ), z );
		
		p[ 0 ]	= radius;
		p[ 1 ]	= theta;
		p[ 2 ]	= phi;
		
		return p;
	}
	
	public static float[] eulerToMatrix ( float degreesX, float degreesY, float degreesZ )
	{
		float angleX;
		float angleY;
		float angleZ;
		
		angleX	= degreesX * TrigUtil.DEGTORAD;
		angleY	= degreesY * TrigUtil.DEGTORAD;
		angleZ	= degreesZ * TrigUtil.DEGTORAD;
		
		float A, B, C, D, E, F, AD, BD;
		float[] m;
		
		A	= (float)Math.cos( angleX );
		B	= (float)Math.sin( angleX );
	    C	= (float)Math.cos( angleY );
	    D	= (float)Math.sin( angleY );
	    E	= (float)Math.cos( angleZ );
	    F	= (float)Math.sin( angleZ );
	    AD	= A * D;
	    BD	= B * D;
	    
	    m		= new float[16];
	    m[0]	= C * E;
	    m[1]	= -C * F;
	    m[2]	= D;
	    m[4] 	= BD * E + A * F;
	    m[5]	= -BD * F + A * E;
	    m[6]	= -B * C;
	    m[8]	= -AD * E + B * F;
	    m[9]	= AD * F + B * E;
	    m[10]	=  A * C;
	    m[3]	= m[7] = m[11] = m[12] = m[13] = m[14] = 0;
	    m[15]	= 1;
	    
	    return m;
	}
	
	public static float[] quaternionToMatrix ( float x, float y, float z, float w )
	{
		float xx, xy, xz, xw, yy, yz, yw, zz, zw;
		float[] m;
		
	    xx	= x * x;
	    xy	= x * y;
	    xz	= x * z;
	    xw	= x * w;
	    yy	= y * y;
	    yz	= y * z;
	    yw	= y * w;
	    zz	= z * z;
	    zw	= z * w;
	    
	    m		= new float[ 16 ];
	    m[0]	= 1 - 2 * ( yy + zz );
	    m[1]	= 2 * ( xy - zw );
	    m[2]	= 2 * ( xz + yw );
	    m[4]	= 2 * ( xy + zw );
	    m[5]	= 1 - 2 * ( xx + zz );
	    m[6]	= 2 * ( yz - xw );
	    m[8]	= 2 * ( xz - yw );
	    m[9]	= 2 * ( yz + xw );
	    m[10]	= 1 - 2 * ( xx + yy );
	    m[3]	= m[7] = m[11] = m[12] = m[13] = m[14] = 0;
	    m[15]	= 1;
	    
	    return m;
	}
}
