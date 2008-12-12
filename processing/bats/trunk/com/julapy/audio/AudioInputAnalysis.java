package com.julapy.audio;

import processing.core.PApplet;
import krister.Ess.AudioChannel;
import krister.Ess.AudioInput;
import krister.Ess.AudioStream;
import krister.Ess.Ess;
import krister.Ess.FFT;

public class AudioInputAnalysis 
{
	PApplet papp;
	AudioInput audioInput;
	AudioStream audioStream;
	FFT fft;
	
	public float fftLevelThreshold	= 0.70f;
	float fftLevel			= 0;
	float fftLevelMax		= 0;
	float fftScale			= 2;
	int sampleCount			= 512;
	
	public AudioInputAnalysis( PApplet papp )
	{
		this.papp = papp;
		
		Ess.start( papp );

		audioInput 	= new AudioInput( );
		audioStream	= new AudioStream( audioInput.size );
		
		audioInput.start();
		audioStream.start();
		
		fft = new FFT( sampleCount );
		fft.equalizer( true );
		fft.smooth = true;
//		fft.damp( 0.75f );
	}
	
	public void update ()
	{
		float currLevel;
		
		fft.getSpectrum( audioInput );
		
		currLevel = fft.getLevel( audioInput ) * fftScale;
		currLevel = Math.max( 0, currLevel - fftLevelThreshold );
		currLevel = Math.min( 1, currLevel );
		
		if( currLevel > fftLevelMax  )
		{
			fftLevelMax = currLevel;
		}
		else
		{
			fftLevelMax	*= 0.95f;
		}
		
		fftLevel += ( fftLevelMax - fftLevel ) * 0.5f;
	}
	
	public float getLevel ()
	{
		return fftLevel;
	}
	
	public void draw () 
	{
		float val;
		float w = 0, h = 0;
		int i;
		
		papp.noStroke();
		papp.fill( 1, 1, 1, 0.8f );

		w = papp.width / (float)fft.spectrum.length;
		
		for ( i=0; i<fft.spectrum.length; i++ ) 
		{
			val = fft.spectrum[ i ] * papp.height * fftScale;
			
			papp.rect
				(
					i * w,
					papp.height - val,
					w,
					val 
				);
		}
		
		h = papp.height - fftLevel * papp.height;
		
		papp.noFill();
		papp.strokeWeight( 3 ); 
		papp.stroke( 1, 0.82f, 0 );
		papp.line( 0, h, papp.width, h );
	}
}
