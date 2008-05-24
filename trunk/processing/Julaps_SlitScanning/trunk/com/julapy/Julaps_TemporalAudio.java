package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_SlitScanning
// File: 			Julaps_TemporalAudio.java
// Created by: 		lukaszkarluk, May 24, 2008
//
////////////////////////////////////////////////////////////////////////////////

import krister.Ess.AudioChannel;
import krister.Ess.Ess;
import krister.Ess.FFT;
import processing.core.*;
import processing.video.Movie;

public class Julaps_TemporalAudio extends PApplet 
{
	static int DIR_X = 1;
	static int DIR_Y = 2;
	
	Movie mov;
	PImage[] frameBuffer;
	int movWidth;
	int movHeight;
	int movFrameRate = 25;
	int movIndex = 0;
	int slitDir;
	int slitSize = 1;
	boolean isRecording = false;
	
	AudioChannel channel;
	FFT fft;
	int sampleCount = 1024;
	float sampleCutoff = 0.73f;
	float[] sampleData;

	public void setup() 
	{
		frameRate( 25 );
		
		//__________________________________________________________Movie.
		
		mov = new Movie( this, "jellyfish.mov", movFrameRate );
		mov.loop();
		mov.speed( 0 );
		mov.read();

		movWidth = mov.width;
		movHeight = mov.height;

		size( movWidth, movHeight, P2D );
		
		frameBuffer = new PImage[ (int)(25 * mov.duration()) ];
		
		println( "frameBuffer length :: " + frameBuffer.length + ", movWidth :: " + movWidth + ", movHeight :: " + movHeight );
		
		for( int i=0; i<frameBuffer.length; i++ )
		{
			mov.jump( i/(float)25 );
			mov.read();
			
			frameBuffer[ i ] = mov.get();
			
			println( "adding frame to buffer :: " + i );
		}
		
		mov.dispose();
		mov = null;

		//__________________________________________________________ESS.

//		size( 512, 400, P2D );
		
		Ess.start(this);

		channel = new AudioChannel("audio/dat_politics_nude_noodle.mp3");
		channel.play( Ess.FOREVER );
		
		fft=new FFT( sampleCount );
		fft.equalizer( true );
		fft.smooth = true;
		fft.damp( 0.5f );
		
		//__________________________________________________________Init.
		
		slitDir = DIR_X;
	}

	public void draw() 
	{
		background( 0 );

		getSampleData( );
		drawVideo( );
//		drawSamples( );
		
		if( isRecording ) save("data/export/export"+frameCount+".png");
	}
	
	public void drawVideo ()
	{
		loadPixels();
		
		if( slitDir == DIR_Y )
		{
			for( int i=0; i<movHeight; i++ )
			{
				int index = (int)( (frameBuffer.length-1) * (sampleData[i]) );
				image( frameBuffer[index].get( 0, i*slitSize, movWidth, slitSize), 0, i*slitSize );
			}
		}

		if( slitDir == DIR_X )
		{
			for( int i=0; i<movWidth; i++ )
			{
				int index = (int)( (frameBuffer.length-1) * (sampleData[i]) );
				println(index);
				image( frameBuffer[index].get( i*slitSize, 0, slitSize, movHeight), i*slitSize, 0 );
			}
		}
		
		updatePixels();
	}
	
	public void getSampleData ()
	{
		fft.getSpectrum(channel);
		
		int sampleDataSize = width;
		if( slitDir == DIR_X ) sampleDataSize = movWidth;
		if( slitDir == DIR_Y ) sampleDataSize = movHeight;
		sampleData = new float[sampleDataSize];
		
		float[] spectrum = new float[ (int)(sampleCount * sampleCutoff) ];
		for( int i=0; i<spectrum.length; i++ )
		{
			if( i < spectrum.length/2 )
			{
				spectrum[i] = fft.spectrum[i];
			}
			else
			{
				int j = spectrum.length - 1 - i;
				spectrum[i] = fft.spectrum[j];
			}
		}
		
		for( int k=0; k<sampleDataSize; k++ )
		{
			int j = (int)(k / (float)sampleDataSize * (spectrum.length - 1)) ;
			sampleData[k] = min( 1, spectrum[j] * 10 );
		}
	}
	
	public void drawSamples() 
	{
		noStroke();
		fill( 255 );

		for (int i=0; i<sampleData.length; i++) 
		{
			float val = max( 0, sampleData[i] * height );
			rect( i, height/2-val, 1, val );
		}
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

	public void stop() 
	{
		Ess.stop();
		super.stop();
	}
}