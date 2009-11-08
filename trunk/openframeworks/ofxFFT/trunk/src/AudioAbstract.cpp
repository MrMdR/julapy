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
		
		fftData[ i ] = fftData[ j - i ]  = fftMagnitude[ i ];
		fftData[ i ] = fftData[ j - i ] *= linearEQIntercept + p * linearEQSlope;
		
		if( fftDataMax[ i ] < fftData[ i ] )
		{
			fftDataMax[ i ] = fftDataMax[ j - i ] = fftData[ i ];
		}
		
		fftData[ i ] = fftData[ j - i ] = fftData[ i ] / fftDataMax[ i ];
		
		fftDataPeak[ i ] = fftDataPeak[ j - i ] *= 0.96;
		if( fftDataPeak[ i ] < fftData[ i ] )
			fftDataPeak[ i ] = fftDataPeak[ j - i ] = fftData[ i ];
		
		if( fftDataPeak[ i ] < threshold )
			fftDataSwitch[ i ] = fftDataSwitch[ j - i ] = 1;
		else
			fftDataSwitch[ i ] = fftDataSwitch[ j - i ] = 0;
		
		fftAveragePeak += fftDataPeak[ i ];
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

void AudioAbstract :: getGlitchData( int *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j = (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBands - 1 ) );
		data[ i ] = fftDataSwitch[ j ];
	}
}

void  AudioAbstract :: setPosition( float value ) {};
float AudioAbstract :: getPosition() { return 0; };

void  AudioAbstract :: setVolume( float value ) {};
float AudioAbstract :: getVolume() { return 0; };