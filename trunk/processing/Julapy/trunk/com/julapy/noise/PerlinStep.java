package com.julapy.noise;

import toxi.math.MathUtils;
import toxi.math.noise.PerlinNoise;

public class PerlinStep
{
	private PerlinNoise	pn;
	public float[] perlinSteps	= {};
	private float perlinVal 	= 0;
	private float perlinSeed	= 0;
	private float perlinRes		= 1.5f;
	private int perlinCount		= 0;
	private int perlinCue		= 10;
	private int perlinCueMin	= 1;
	private int perlinCueMax	= 1;
	
	public PerlinStep( int stepsTotal, float perlinRes, int perlinCueMin, int perlinCueMax )
	{
		this.perlinRes		= perlinRes;
		this.perlinCueMin	= perlinCueMin;
		this.perlinCueMax	= perlinCueMax;	
		
		int i;
		
		pn = new PerlinNoise();
		
		perlinSteps = new float[ stepsTotal ];
		for( i=perlinSteps.length-1; i>=0; i-- )
		{
			progressValue();
			
			perlinSteps[ i ] = perlinVal;
		}
	}
	
	private void progressValue ()
	{
		if( perlinCount++ == perlinCue )
		{
			perlinVal			= pn.noise( perlinSeed += perlinRes ) * 3 - 1.0f;
		    perlinVal			= (int)( perlinVal * 10 ) / (float)10;
		    perlinVal			= MathUtils.max( perlinVal, 0.0f );
		    perlinVal			= MathUtils.min( perlinVal, 1.0f );

		    perlinCue			+= (int)MathUtils.random( perlinCueMin, perlinCueMax );
		}
	}
	
	public void update ()
	{
		int i;
		
	    for( i=perlinSteps.length-1; i>0; i-- )
	    {
	    	perlinSteps[ i ] = perlinSteps[i-1];
	    }

	    progressValue();
	    
	    perlinSteps[ i ] = perlinVal;
	    
//		    println( perlinVal );
	}
}
