package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			CilindricoCollapse_PerlinStep.java
// Created by: 		lukaszkarluk, Mar 1, 2009
//
////////////////////////////////////////////////////////////////////////////////

import processing.core.*;
import toxi.math.MathUtils;
import toxi.math.noise.PerlinNoise;

public class CilindricoCollapse_PerlinStep extends PApplet 
{
	PerlinStep ps01;
	PerlinStep ps02;
	
	public void setup() 
	{
		size( 720, 360 );
		frameRate( 25 );
		colorMode( RGB, 1.0f );

		ps01 = new PerlinStep( 360, 1.5f, 5, 30 );
		ps02 = new PerlinStep( 360, 1.5f, 1, 1 );
	}

	public void draw() 
	{
		background(0);

		ps01.update();
		ps02.update();
		
		drawPerlinSteps();
	}
	
	public void keyPressed()
	{
		if( key == 'r' )
		{
			save("export" + frameCount + ".png");
		}
	}
		
	private void drawPerlinSteps ()
	{
		float w;
		int i;
		
		fill( 0.5f );

		// perlin step 01.
		
		w = width / (float)ps01.perlinSteps.length;
		
		for( i=0; i<ps01.perlinSteps.length; i++ )
		{
			rect
			( 
				i * w,
				height * 0.5f, 
				w,
				-ps01.perlinSteps[ i ] * height * 0.5f
			);
		}

		// perlin step 02.
		
		w = width / (float)ps02.perlinSteps.length;
		
		for( i=0; i<ps02.perlinSteps.length; i++ )
		{
			rect
			( 
				i * w,
				height,
				w,
				-ps02.perlinSteps[ i ] * height * 0.5f
			);
		}
		
	}
	
	public class PerlinStep
	{
		PerlinNoise	pn;
		float[] perlinSteps	= {};
		float perlinVal 	= 0;
		float perlinSeed	= 0;
		float perlinRes		= 1.5f;
		int perlinCount		= 0;
		int perlinCue		= 10;
		int perlinCueMin	= 1;
		int perlinCueMax	= 1;
		
		public PerlinStep( int stepsTotal, float perlinInc, int perlinCueMin, int perlinCueMax )
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
}