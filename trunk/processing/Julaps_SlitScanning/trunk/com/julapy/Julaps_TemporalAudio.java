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
	int movDir = 1;
	int slitDir;
	int slitSize = 1;
	boolean isRecording = false;
	
	AudioChannel channel;
	FFT fft;
	float fftLevel = 0;
	int sampleCount = 1024;
	float sampleCutoff = 0.73f;
	float[] sampleData;

	public void setup() 
	{
		frameRate( 25 );
		
		initVideo();
		initEss();
		
		slitDir = DIR_X;
	}
	
	public void initVideo ()
	{
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
			
			if( i % 50 == 0 && i != 0 ) println( i + " frames added to buffer" ); 
		}
		
		mov.dispose();
		mov = null;
	}
	
	public void initEss ()
	{
//		size( 512, 400, P2D );
		
		Ess.start( this );

		channel = new AudioChannel( "audio/autechre_zeiss_contarex_trim.aif" );
		channel.play( );
		
		fft = new FFT( sampleCount );
		fft.equalizer( true );
		fft.smooth = true;
		fft.damp( 0.5f );
	}

	public void draw() 
	{
		background( 0 );

		getLevel( );
		getSampleData( );
		drawVideo( );
//		drawSamples( );
		
		if( isRecording ) save("data/export/export"+frameCount+".png");
	}
	
	public void drawVideo ()
	{
		movIndex += movDir;
		if( movIndex > frameBuffer.length - 1 ) movIndex = 0;
		if( movIndex < 0 ) movIndex = frameBuffer.length - 1;
		
		if( slitDir == DIR_Y )
		{
			for( int i=0; i<movHeight; i++ )
			{
				int index = (int)( (frameBuffer.length-1) * (sampleData[i]) );
				index += movIndex;
				if( index >= frameBuffer.length ) index -= frameBuffer.length;
				
				image( frameBuffer[index].get( 0, i*slitSize, movWidth, slitSize), 0, i*slitSize );
			}
		}

		if( slitDir == DIR_X )
		{
			for( int i=0; i<movWidth; i++ )
			{
				int index = (int)( (frameBuffer.length-1) * (sampleData[i]) );
				index += movIndex;
				if( index >= frameBuffer.length ) index -= frameBuffer.length;
				
				image( frameBuffer[index].get( i*slitSize, 0, slitSize, movHeight), i*slitSize, 0 );
			}
		}
	}
	
	public void getLevel ()
	{
		float fftChannelLevel = fft.getLevel( channel );
		
		if( fftChannelLevel > fftLevel )
		{
			fftLevel = fftChannelLevel;
			
			if( random( 1 ) < 0.2f ) 
			{
				movDir *= -1;
				
				println( "movDir :: " + movDir );
			}
			
			if( random( 1 ) < 0.9f )
			{
				movIndex += (int)( random( ( frameBuffer.length - 1 ) * 0.2f, ( frameBuffer.length - 1 ) * 0.5f ) ) * movDir;
				if( movIndex > frameBuffer.length - 1 ) movIndex = movIndex - ( frameBuffer.length - 1 );
				if( movIndex < 0 ) movIndex = frameBuffer.length - 1 + movIndex;
				
				println( "skip to random." );
			}
		}
		else
		{
			fftLevel *= 0.995;
		}
		
		if( fftChannelLevel == 0 ) movDir = 1;
	}
	
	public void getSampleData ()
	{
		fft.getSpectrum(channel);
		
		int sampleDataSize = width;
		if( slitDir == DIR_X ) sampleDataSize = width;
		if( slitDir == DIR_Y ) sampleDataSize = height;
		sampleData = new float[sampleDataSize];
		
		float[] spectrum = new float[ (int)(sampleCount * sampleCutoff) ];
		for( int i=0; i<spectrum.length; i++ )
		{
			if( i < (int)(spectrum.length/2) )
			{
				int j = (int)(spectrum.length/2) - 1 - i;
				spectrum[i] = fft.spectrum[j];
			}
			else
			{
				int j = i - (int)(spectrum.length/2);
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
		fill( 255, 255, 255, 100 );

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