package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_SlitScanning
// File: 			Julaps_SlitTemporalWaves.java
// Created by: 		lukaszkarluk, May 21, 2008
//
////////////////////////////////////////////////////////////////////////////////

import processing.core.PApplet;
import processing.core.PImage;
import processing.video.Movie;

public class TemporalNoise extends PApplet
{
	Movie mov;
	PImage[] frameBuffer;
	int movWidth;
	int movHeight;
	int movFrameRate = 25;
	boolean isRecording = false;
	
	float noiseCount = 0.0f;
	float noiseSpeed = 0.03f;
	float noiseScale = 0.005f;
	float noiseValMin = 0.5f;
	float noiseValMax = 0.5f;
	
	public void setup()
	{
		frameRate( 25 );

		mov = new Movie( this, "blossom.mov", movFrameRate );
		mov.loop();
		mov.speed( 0 );
		mov.read();

		movWidth = mov.width;
		movHeight = mov.height;

		size( movWidth, movHeight, P2D );
		
		frameBuffer = new PImage[ (int)(25 * mov.duration()) ];
//		frameBuffer = new PImage[ movHeight ];
		
		println( "frameBuffer length :: " + frameBuffer.length + ", movWidth :: " + movWidth + ", movHeight :: " + movHeight );
		
		for( int i=0; i<frameBuffer.length; i++ )
		{
			mov.jump( i/(float)25 );
			mov.read();
			
			frameBuffer[ i ] = mov.get();
		}
		
		mov.dispose();
		mov = null;
	}

	public void draw() 
	{
		background( 0 );
		
		loadPixels();
		
		for(int y=0; y<movHeight; y++) 
		{
			for(int x=0; x<movWidth; x++) 
			{
				float noiseVal = noise( ( x ) * noiseScale, y * noiseScale, noiseCount );
				if( noiseVal < noiseValMin ) 
				{
					noiseValMin = noiseVal;
					println("new min noise value :: " + noiseValMin );
				}
				if( noiseVal > noiseValMax ) 
				{
					noiseValMax = noiseVal;
					println("new max noise value :: " + noiseValMax );
				}
				noiseVal = ( noiseVal - noiseValMin ) / ( noiseValMax - noiseValMin );	// scale to between 0 - 1
				
				int fi = (int)( (frameBuffer.length-1) * noiseVal );	// frame index.
				int pi = y * width + x; 								// pixel index.
				
				pixels[ pi ] = frameBuffer[ fi ].pixels[ pi ];
			}
		}
		
		noiseCount += noiseSpeed;
		
		updatePixels();

		if( isRecording ) save("data/export/export"+frameCount+".png");
	}
	
	public void keyPressed()
	{
		if(key == 's')
		{
			save("data/screengrab"+frameCount+".png");
		}
		
		if( key == 'r' )
		{
			isRecording = !isRecording;
		}
	}
}