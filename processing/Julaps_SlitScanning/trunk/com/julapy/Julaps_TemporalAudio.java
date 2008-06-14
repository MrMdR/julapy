package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_SlitScanning
// File: 			Julaps_TemporalAudio.java
// Created by: 		lukaszkarluk, May 24, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.io.PrintWriter;
import java.util.ArrayList;

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
	boolean isRecording = true;
	
	AudioChannel channel;
	FFT fft;
	float fftLevel = 0;
	float fftChannelLevel = 0;
	int sampleCount = 1024;
	float sampleCutoff = 0.73f;
	float[] sampleData;
	int sampleDataSize;
	ArrayList<Slice> slices;
	
	float[][] audioData;
	PrintWriter audioDataWriter;
	
	int frameNumber = 0;
	int framesPerSec = 25;

	public void setup() 
	{
		frameRate( framesPerSec );
		
		initVideo();
		
		slitDir = DIR_X;
		sampleDataSize = width;
		if( slitDir == DIR_X ) sampleDataSize = width;
		if( slitDir == DIR_Y ) sampleDataSize = height;

		initEss();
		
		slices = new ArrayList<Slice>();
	}
	
	public void initVideo ()
	{
		mov = new Movie( this, "jellyfishsky.mov", movFrameRate );
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
		
		if( !isRecording )
			channel.play( );
		
		fft = new FFT( sampleCount );
		fft.equalizer( true );
		fft.smooth = true;
		fft.damp( 0.5f );
	}

	
	public void draw() 
	{
		background( 0 );

		getAudioData( );
		getLevel( );
		getSampleData( );
		drawVideo( );
//		drawSamples( );
		drawSlices( );
		
		if( isRecording ) save("data/export/export"+frameNumber+".png");
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
	
	public void getAudioData ()
	{
		if( isRecording )
		{
			int offset = (int)(frameNumber * channel.sampleRate / framesPerSec);
			fft.getSpectrum( channel.samples, offset );
			
			float totalSpec = 0;
			for( int i=0; i<fft.spectrum.length; i++ )
			{
				totalSpec += fft.spectrum[i];
			}
			
			fftChannelLevel = totalSpec / fft.spectrum.length * 100;

			frameNumber++;
		}
		else
		{
			fft.getSpectrum( channel );
			fftChannelLevel = fft.getLevel( channel );
			
			float totalSpec = 0;
			for( int i=0; i<fft.spectrum.length; i++ )
			{
				totalSpec += fft.spectrum[i];
			}
			
			fftChannelLevel = totalSpec / fft.spectrum.length * 100;
		}
	}
	
	public void getLevel ()
	{
		if( fftChannelLevel > fftLevel )
		{
			float diffLevel = fftChannelLevel - fftLevel;

			fftLevel = fftChannelLevel;
			
			if( random( 1 ) < 0.3f ) 
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
			
			if( random( 1 ) < 0.8f )
			{
				int slicesPerBurst = (int)(diffLevel*100);
				
				addSlices( slicesPerBurst );
				
				println("add slices :: " + slicesPerBurst);
			}
		}
		else
		{
			fftLevel *= 0.99;
		}
		
		if( fftChannelLevel == 0 ) movDir = 1;
	}
	
	
	public void getSampleData ()
	{
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
	
	public void addSlices( int slicesPerBurst )
	{
		for( int i=0; i<slicesPerBurst; i++ )
		{
			slices.add( new Slice( ) );
		}
	}
	
	public void drawSlices()
	{
		for( int i=0; i<slices.size(); i++ )
		{
			Slice slice = slices.get( i );
			slice.update();
			if( slice.alive ) 
			{
				slice.render();
			}
			else
			{
				slices.remove( i );
				--i;
			}
		}
	}
	
	//_______________________________________________________________________Audio.
	
//	public void loadAudioData ()
//	{
//		String[] audioDataStr = loadStrings( "data/audiodata.txt"  );
//		
//		audioData = new float[audioDataStr.length][sampleDataSize+1];	// added the extra 1 for fftLevel data.
//		for( int i=0; i<audioData.length; i++ )
//		{
//			String audioDataLine	= audioDataStr[i];
//			String[] audioDataSplit = split( audioDataLine, '\t'); 
//			
//			for( int j=0; j<audioDataSplit.length; j++)
//			{
//				audioData[i][j] = Float.valueOf( audioDataSplit[j] );
//			}
//		}
//	}
//	
//	public void saveAudioData ()
//	{
//		for( int i=0; i<sampleData.length; i++ )
//		{
//			audioData[frameCount-1][i] = sampleData[i];
//		}
//		audioData[frameCount-1][audioData.length-1] = fftChannelLevel;
//	}
//	
//	public void exportAudioData ()
//	{
//		audioDataWriter = createWriter("data/audiodata.txt");
//		
//		for(int i=0; i<audioData.length; i++)
//		{
//			String values = "";
//			
//			for(int j=0; j<audioData[i].length; j++)
//			{
//				values += audioData[i][j];
//				if( j == audioData[i].length - 1 ) values += "\t";
//			}
//			
//			audioDataWriter.println( values );
//		}
//	}
//	
//	public void readAudioData ()
//	{
//		sampleData = new float[sampleDataSize];
//		for( int i=0; i<sampleData.length; i++ )
//		{
//			sampleData[i] = audioData[frameCount-1][i];
//		}
//		
//		fftChannelLevel = sampleData[sampleData.length-1];
//	}
	
	//_______________________________________________________________________ Keys.
	
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
		
		if( key == 'w' )
		{
//			exportAudioData();
		}
	}

	public void stop() 
	{
		Ess.stop();
		super.stop();
	}
	
	public class Slice
	{
		PImage sliceImage;
		float lifetime = 50;
		float lifetimeDec = 1;
		float x;
		float y;
		int w;
		int h;
		int sx;
		int sy;
		int sw;
		int sh;
		int sliceIndex = 0;
		int sliceDir = 1;
		float sliceSpeed = 2.5f;
		boolean alive = true;
		
		public Slice ()
		{
			w = (int)random( width * 0.2f, width * 0.7f );
			h = (int)random( 8, 40 );

			sliceDir = ( random( 1 ) < 0.5f ) ? 1 : -1;
			
			x = ( sliceDir == 1 ) ? -w : width;
			y = random( height - h );
			
			sliceIndex = (int)random( frameBuffer.length - 1 );
			sliceSpeed = random( 40, 60 );
		}
		
		public void update ()
		{
//			lifetime -= lifetimeDec;
			x += sliceSpeed * sliceDir;

			if( sliceDir == 1 && x > width ) 
			{
				alive = false;
				return;
			}
			if( sliceDir == -1 && x < -w ) 
			{
				alive = false;
				return;
			}
			if( lifetime < 0 ) 
			{
				alive = false;
				return;
			}
			
			sx = (int)x;
			sy = (int)y;
			sw = (int)w;
			sh = (int)h;

			if( sx < 0 )
			{
				sw += sx;
				sx = 0;
			}
		
			if( sx + sw > width )
			{
				sw -= sx + sw - width;
			}
			
			sliceIndex += sliceDir;
			if( sliceIndex > frameBuffer.length - 1 ) sliceIndex = 0;
			if( sliceIndex < 0 ) sliceIndex = frameBuffer.length - 1;
		}
		
		public void render ()
		{
			image( frameBuffer[movIndex].get( sx, sy, sw, sh ), sx, sy );
		}
	}
}