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
	
	float fftLevel;
	float fftScale	= 5;
	int sampleCount	= 1024;
	
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
		fft.damp( 0.5f );
	}
	
	public void update ()
	{
		fft.getSpectrum( audioInput );
		
		fftLevel = fft.getLevel( audioInput );
		fftLevel *= fftScale;
	}
	
	public void draw () 
	{
		float val;
		float w = 0;
		int i;
		
		papp.noStroke();
		papp.fill( 1, 1, 1, 0.8f );

		w = papp.width / (float)fft.spectrum.length;
		
		for ( i=0; i<fft.spectrum.length; i++ ) 
		{
			val = Math.max( 0, fft.spectrum[ i ] * papp.height * fftScale );
			
			papp.rect
				(
					i * w,
					papp.height - val,
					w,
					val 
				);
		}
	}
}
