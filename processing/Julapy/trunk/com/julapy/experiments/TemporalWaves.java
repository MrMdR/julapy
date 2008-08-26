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

public class TemporalWaves extends PApplet
{
	static int DIR_X = 1;
	static int DIR_Y = 2;
	
	Movie mov;
	PImage[] frameBuffer;
	int slitX = 0;
	int slitY = 100;
	int slitIndex = 0;
	int slitSize = 1;
	int slitDir;
	int movWidth;
	int movHeight;
	int movFrameRate = 25;
	float timeWavesPerCycle = 0.5f;
	float timeWaveSpeed = 2 * PI * 0.01f;
	float anglePerFrame;
	float angle = 0;
	boolean isRecording = false;
	
	public void setup() 
	{
		frameRate( 25 );

		colorMode(RGB, 1.0f);
		
		mov = new Movie( this, "decompose.mov", movFrameRate );
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
			
			if( i % 50 == 0 && i != 0 ) println( i + " frames in buffer" );
		}
		
		mov.dispose();
		mov = null;
		
		anglePerFrame =  timeWavesPerCycle * 2 * PI / movHeight;
		
		slitDir = DIR_X;
	}

	public void draw() 
	{
		background( 0 );
		
		loadPixels();
		
		if( slitDir == DIR_Y )
		{
			for( int i=0; i<movHeight; i++ )
			{
				int index = (int)( (frameBuffer.length-1) * (0.5 * sin( angle + anglePerFrame * i ) + 0.5 ) );
				image( frameBuffer[index].get( 0, i*slitSize, movWidth, slitSize), 0, i*slitSize );
			}
		}

		if( slitDir == DIR_X )
		{
			for( int i=0; i<movWidth; i++ )
			{
				int index = (int)( (frameBuffer.length-1) * (0.5 * sin( angle + anglePerFrame * i ) + 0.5 ) );
				image( frameBuffer[index].get( i*slitSize, 0, slitSize, movHeight), i*slitSize, 0 );
			}
		}
		
		updatePixels();

		angle += timeWaveSpeed;
		
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