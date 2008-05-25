package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_SlitScanning
// File: 			Julaps_TemporalHole.java
// Created by: 		lukaszkarluk, May 22, 2008
//
////////////////////////////////////////////////////////////////////////////////

import processing.core.*;
import processing.video.Movie;

public class Julaps_TemporalHole extends PApplet 
{
	Movie mov;
	PImage[] frameBuffer;
	int movWidth;
	int movHeight;
	int movFrameRate = 25;
	int movIndex = 0;
	boolean isRecording = false;
	
	float holeDepth = 0;
	float holeRadius = 0;
	float holeRadiusMax;

	public void setup() 
	{
		frameRate( 25 );
		
		mov = new Movie( this, "flocking.mov", movFrameRate );
		mov.loop();
		mov.speed( 0 );
		mov.read();

		movWidth = mov.width;
		movHeight = mov.height;

		holeRadiusMax = ( movWidth > movHeight ) ? movWidth/2 : movHeight/2;
		
		size( movWidth, movHeight, P2D );
		
		frameBuffer = new PImage[ (int)(25 * mov.duration()) ];
		
		println( "frameBuffer length :: " + frameBuffer.length + ", movWidth :: " + movWidth + ", movHeight :: " + movHeight );
		
		for( int i=0; i<frameBuffer.length; i++ )
		{
			mov.jump( i/(float)25 );
			mov.read();
			
			frameBuffer[ i ] = mov.get();
			
			if( i % 50 == 0 && i != 0 ) println( i + " frames in buffer" );
		}
		
		mov.dispose();
		mov = null;
	}

	public void draw() 
	{
		background( 255 );

		if( mousePressed )
		{
			if( holeDepth == 0 ) holeDepth = 0.1f;
			else holeDepth *= 1.1;
			if( holeDepth > 1 ) holeDepth = 1;
		}
		else
		{
			holeDepth *= 0.9;
			if( holeDepth < 0.01 ) holeDepth = 0;
		}
		
		holeRadius = holeDepth * holeRadiusMax;
		
		image( frameBuffer[movIndex++], 0, 0 );
		if( movIndex == frameBuffer.length ) movIndex = 0;
		
		loadPixels();
		
		for(int y=0; y<movHeight; y++) 
		{
			for(int x=0; x<movWidth; x++)
			{
				float dx = x - width/2;
				float dy = y - height/2;
				float d = sqrt( dx*dx + dy*dy );
				if( d < holeRadius )
				{
//					float ds = linear( d );		// distortion scale.
					float ds = singraph( d );
					
					int fi = (int)( (frameBuffer.length-1) * ds );	// frame index.
					fi += movIndex;
					if( fi >= frameBuffer.length ) fi -= frameBuffer.length;
					
					int pi = y * width + x; 						// pixel index.

					pixels[ pi ] = frameBuffer[ fi ].pixels[ pi ];
				}
			}
		}
		
		updatePixels();

		if( isRecording ) save("data/export/export"+frameCount+".png");
	}
	
	public float linear ( float v )
	{
		return holeDepth / holeRadius * ( holeRadius - v );
	}
	
	public float singraph ( float v )	// TODO :: still needs some work. 
	{
		v = v / holeRadius;	// scale from 0 to 1
		v = ( v - 0.5f ) * 1.0f; 			// scale from -1 to 1
		v = sin( v * PI ) / 2 + 0.5f;
		v = v * holeDepth;
		return v;
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