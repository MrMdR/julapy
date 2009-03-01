package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			CilindricoCollapse_PerlinStep.java
// Created by: 		lukaszkarluk, Mar 1, 2009
//
////////////////////////////////////////////////////////////////////////////////

import javax.media.opengl.GL;
import processing.opengl.PGraphicsOpenGL;
import processing.core.*;

public class CilindricoCollapse_PerlinStep extends PApplet 
{
	float[] perlinSteps = {};
	float perlinVal 	= 0;
	float perlinSeed	= 0;
	float perlinInc		= 1.5f;
	int perlinCount		= 0;
	int perlinCue		= 10;

	public void setup() 
	{
		size( 720, 360 );
		frameRate( 25 );
		colorMode( RGB, 1.0f );

		initPerlinSteps();
	}

	public void draw() 
	{
		background(0);

		updatePerlinSteps();
		drawPerlinSteps();
	}
	
	private void initPerlinSteps()
	{
		int i;
		
		perlinSteps = new float[ 360 ];
		for( i=perlinSteps.length-1; i>=0; i-- )
		{
			if( perlinCount++ == perlinCue )
			{
			    perlinVal			= noise( perlinSeed += perlinInc ) * 3 - 1.0f;
			    perlinVal			= (int)( perlinVal * 10 ) / (float)10;
			    perlinVal			= max( perlinVal, 0.0f );
			    perlinVal			= min( perlinVal, 1.0f );

			    perlinCue			+= (int)random( 5, 30 );
			}
			
			perlinSteps[ i ] = perlinVal;
		}
	}
	
	private void updatePerlinSteps ()
	{
		int i;
		
	    for( i=perlinSteps.length-1; i>0; i-- )
	    {
	    	perlinSteps[ i ] = perlinSteps[i-1];
	    }

		if( perlinCount++ == perlinCue )
		{
		    perlinVal			= noise( perlinSeed += perlinInc ) * 3 - 1.0f;
		    perlinVal			= (int)( perlinVal * 10 ) / (float)10;
		    perlinVal			= max( perlinVal, 0.0f );
		    perlinVal			= min( perlinVal, 1.0f );
		    perlinSteps[ 0 ]	= perlinVal;
		    
		    perlinCue			+= (int)random( 5, 30 );
		}
	    
	    println( perlinVal );
	}
	
	private void drawPerlinSteps ()
	{
		float w;
		int i;
		
		w = width / (float)perlinSteps.length;
		
		fill( 0.5f );
		
		for( i=0; i<perlinSteps.length; i++ )
		{
			rect
			( 
				i * w,
				height * 0.5f, 
				w,
				-perlinSteps[ i ] * height * 0.5f
			);
		}
	}
}