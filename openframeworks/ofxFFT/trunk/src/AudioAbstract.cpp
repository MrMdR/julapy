/*
 *  AudioAbstract.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioAbstract.h"

AudioAbstract :: AudioAbstract() {};
AudioAbstract :: ~AudioAbstract() {};

void  AudioAbstract :: init( string fileName ) 
{
	setNoOfBands( 128 );
	setFrameRateSync( false );
	setThreshold( 0.5 );
	setPeakDecay( 0.96 );
	setMaxDecay( 1.0 );
	
	linearEQIntercept	= 0.9;
	linearEQSlope		= 1.0;
	
	defaultRenderWidth	= 200;
	defaultRenderHeight = 100;
	
	fftAveragePeakNorm.setUseBuffer( false );
	
	fftAveragePower = 0;
	fftAveragePeak	= 0;
	
	resetFFT();
}

void  AudioAbstract :: update() 
{
	myfft.powerSpectrum( 0, audioNoOfBandsHalf, specData, audioNoOfBands, fftMagnitude, fftPhase, fftPower, &fftAveragePower );
	
	for( int i=0; i<audioNoOfBandsHalf; i++ )
	{
		float p = i / (float)( audioNoOfBands - 1 );
		int j	= audioNoOfBands - 1;
		
		float fftDataVal;
		fftDataVal = fftMagnitude[ i ];								// use magnitude for fft data.
		fftDataVal *= linearEQIntercept + p * linearEQSlope;		// scale value.
		
		float fftDataMaxVal;
		fftDataMaxVal = fftDataMax[ i ];

		if( fftDataMaxVal < fftDataVal )
		{
			fftDataMaxVal = fftDataVal;
		}
		
		float fftDataNormVal;
		fftDataNormVal = 0;
		if( fftDataMaxVal > 0 )
			fftDataNormVal = fftDataVal / fftDataMaxVal;			// normalise data between 0 and 1.
		
		fftDataMaxVal *= fftDataMaxDecay;							// decay the max value.
		
		fftData[ i ]			= fftDataNormVal;
		fftData[ j - i ]		= fftDataNormVal;					// mirror arrays.
		
		fftDataMax[ i ]			= fftDataMaxVal;
		fftDataMax[ j - i ]		= fftDataMaxVal;					// mirror arrays.
		
		float fftDataPeakVal;
		fftDataPeakVal = fftDataPeak[ i ];
		fftDataPeakVal *= fftDataPeakDecay;							// decay peak value.
		
		if( fftDataPeakVal < fftDataNormVal )						// check if new peak.
			fftDataPeakVal = fftDataNormVal;
		
		fftDataPeak[ i ]		= fftDataPeakVal;
		fftDataPeak[ j - i ]	= fftDataPeakVal;
		
		fftAveragePeak			+= fftDataPeakVal;					// sum of all peaks.
		
		int fftDataSwitchVal;										// switch data (on/off).
		if( fftDataPeakVal < threshold )
			fftDataSwitchVal = 1;
		else
			fftDataSwitchVal = 0;
		
		fftDataSwitch[ i ]		= fftDataSwitchVal;
		fftDataSwitch[ j - i ]	= fftDataSwitchVal;
	}
	
	fftAveragePeak /= audioNoOfBandsHalf;
	
	fftAveragePeakNorm.addValue( fftAveragePeak );
}

void  AudioAbstract :: draw( int width, int height ) 
{
	if( width <= 0 )
		width = defaultRenderWidth;
	
	if( height <= 0 )
		height = defaultRenderHeight;
	
	int bx, by;		// border.
	bx =  by = 1;
	
	ofFill();
	
	ofSetColor( 113, 113, 113 );
	ofRect( 0, 0, width + bx * 2, height + by * 2 );
	
	ofSetColor( 232, 232, 232 );
	ofRect( bx, by, width, height );
	
	float singleBandWidth = width / (float)audioNoOfBands;
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		ofFill();
		ofSetColor( 0, 0, 255 );
		ofRect( i * singleBandWidth + bx, height + by, singleBandWidth, -fftDataSwitch[ i ] * height );
	}
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		ofFill();
		ofSetColor( 50, 50, 50 );
		ofRect( i * singleBandWidth + bx, height + by, singleBandWidth, -fftData[ i ] * height );
		
		ofNoFill();
		ofSetColor( 232, 232, 232 );
		ofRect( i * singleBandWidth + bx, height + by, singleBandWidth, -fftData[ i ] * height );
	}
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		ofFill();
		ofSetColor( 255, 0, 0 );
		ofRect( i * singleBandWidth + bx, ( 1 - fftDataPeak[ i ] ) * height + by, singleBandWidth, 2 );
	}
	
	ofNoFill();
	ofSetColor( 255, 0, 255 );
	ofLine( bx, ( 1 - threshold ) * height + by, width + bx, ( 1 - threshold ) * height + by );
	
	ofNoFill();
	ofSetColor( 0, 255, 255 );
	ofLine( bx, ( 1 - fftAveragePeakNorm.getNormalisedValue() ) * height + by, width + bx, ( 1 - fftAveragePeakNorm.getNormalisedValue() ) * height + by );
}

void  AudioAbstract :: resetFFT() 
{
	delete[] specData;
	delete[] fftMagnitude;
	delete[] fftPhase;
	delete[] fftPower;
	delete[] fftFreq;
	delete[] fftData;
	delete[] fftDataMax;
	delete[] fftDataPeak;
	
	specData		= new float[ audioNoOfBands ];
	
	fftMagnitude	= new float[ audioNoOfBands ];
	fftPhase		= new float[ audioNoOfBands ];
	fftPower		= new float[ audioNoOfBands ];
	fftFreq			= new float[ audioNoOfBandsHalf ];
	
	fftData         = new float[ audioNoOfBands ];
	fftDataMax		= new float[ audioNoOfBands ];
	fftDataPeak		= new float[ audioNoOfBands ];
	fftDataSwitch	= new int[ audioNoOfBands ];
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		fftData[ i ]	 = 0.0;
		fftDataMax[ i ]	 = 0.0;
		fftDataPeak[ i ] = 0.0;
	}
}

void AudioAbstract :: setNoOfBands( int value )
{
	audioNoOfBands		= ofNextPow2( value );
	audioNoOfBandsHalf	= (int)( audioNoOfBands * 0.5 );
	
	resetFFT();
}

int AudioAbstract :: getNoOfBands()
{
	return audioNoOfBands;
}

void AudioAbstract :: setFrameRateSync( bool b )
{
	frameRateSync = b;
	
	frameStart = ofGetFrameNum();
}

void AudioAbstract :: setThreshold( float value )
{
	threshold = MIN( 1, MAX( 0, value ) );
}

float AudioAbstract :: getThreshold()
{
	return threshold;
}

float AudioAbstract :: getAveragePeak()
{
	return fftAveragePeak;
}

float AudioAbstract :: getAveragePeakNorm()
{
	return fftAveragePeakNorm.getNormalisedValue();
}

void AudioAbstract :: setPeakDecay ( float value )
{
	value = MIN( 1, MAX( 0, value ) );
	
	fftDataPeakDecay = value;
}

float AudioAbstract :: getPeakDecay ()
{
	return fftDataPeakDecay;
}

void AudioAbstract :: setMaxDecay ( float value )
{
	fftDataMaxDecay = value;
}

float AudioAbstract :: getMaxDecay ()
{
	return fftDataMaxDecay;
}

//////////////////////////////////////////////////////
//	GET VARIOUS DATA SAMPLES.
//////////////////////////////////////////////////////

void AudioAbstract :: getFftData ( float *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBands - 1 ) );
		float v		= fftData[ j ];
		data[ i ]	= v;
	}
}

void AudioAbstract :: getFftPeakData ( float *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBands - 1 ) );
		float v		= fftDataPeak[ j ];
		data[ i ]	= v;
	}
}

void AudioAbstract :: getGlitchData( int *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBands - 1 ) );
		float v		= fftDataSwitch[ j ];
		data[ i ]	= v;
	}
}

//////////////////////////////////////////////////////
//	
//////////////////////////////////////////////////////

void  AudioAbstract :: setPosition( float value ) {};
float AudioAbstract :: getPosition() { return 0; };

void  AudioAbstract :: setVolume( float value ) {};
float AudioAbstract :: getVolume() { return 0; };