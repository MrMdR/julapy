package com.julapy.utils;

import toxi.math.MathUtils;

public class TrigLookUpTable 
{
	public	static final float DEG_TO_RAD = MathUtils.PI / 180.0f;
	
	public	static float sinLUT[];
	public	static float cosLUT[];	
	public	static float SINCOS_PRECISION	= 1.0f;
	private static int SINCOS_LENGTH		=  (int)( 360.0 / SINCOS_PRECISION );
	
	public static void initLookUpTabel ()
	{
		int i;
		
		sinLUT = new float[ SINCOS_LENGTH ];
		cosLUT = new float[ SINCOS_LENGTH ];
		for ( i=0; i<SINCOS_LENGTH; i++ )
		{
			sinLUT[ i ] = (float)Math.sin( i * DEG_TO_RAD * SINCOS_PRECISION );
			cosLUT[ i ] = (float)Math.cos( i * DEG_TO_RAD * SINCOS_PRECISION );
		}
	}
}
