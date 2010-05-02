/*
 *  AudioAbstract.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioAbstract.h"

AudioAbstract ::  AudioAbstract() {};
AudioAbstract :: ~AudioAbstract() {};

void AudioAbstract :: init()
{
	audioNoOfBands		= AUDIO_ABSTRACT_DEFAULT_NO_OF_BANDS;
	audioNoOfBandsHalf	= (int)( audioNoOfBands * 0.5 );
	
	initFFT();
	initAudioData( rawData, getNoOfBands() );
	initAudioData( fftData, getNoOfBands() );
	
	setThreshold( 0.5 );
	setPeakDecay( 0.96 );
	setMaxDecay( 1.0 );
	setMirrorData( false );
	setUseFftData( true );
	
	renderBorder = 1;
}

void AudioAbstract :: update() 
{
	myfft.powerSpectrum( 0, audioNoOfBandsHalf, specData, audioNoOfBands, fftMagnitude, fftPhase, fftPower, &fftAveragePower );
	
	if( bUseFftData )
	{
		updateAudioData( fftData, fftMagnitude );
	}
	else
	{
		updateAudioData( rawData, specData );
	}
	
	if( bMirrorData )
	{
		if( bUseFftData )
		{
			mirrorAudioData( fftData );
		}
		else
		{
			mirrorAudioData( rawData );
		}
	}
}

//////////////////////////////////////////////////////
//	FFT.
//////////////////////////////////////////////////////

void AudioAbstract :: initFFT ()
{
	specData		= new float[ audioNoOfBands ];
	
	fftMagnitude	= new float[ audioNoOfBands ];
	fftPhase		= new float[ audioNoOfBands ];
	fftPower		= new float[ audioNoOfBands ];
	fftFreq			= new float[ audioNoOfBandsHalf ];
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		specData[ i ]		= 0;
		fftMagnitude[ i ]	= 0;
		fftPhase[ i ]		= 0;
		fftPower[ i ]		= 0;
		
		if( i < audioNoOfBandsHalf )
			fftFreq[ i ]	= 0;
	}
}

void AudioAbstract :: resetFFT() 
{
	delete[] specData;
	delete[] fftMagnitude;
	delete[] fftPhase;
	delete[] fftPower;
	delete[] fftFreq;
}

//////////////////////////////////////////////////////
//	AUDIO DATA.
//////////////////////////////////////////////////////

void AudioAbstract :: initAudioData	( AudioData &audioData, int dataSize )
{
	audioData.size				= dataSize;
	
	audioData.data.resize( dataSize );
	audioData.dataNorm.resize( dataSize );
	audioData.dataMax.resize( dataSize );
	audioData.dataPeak.resize( dataSize );
	audioData.dataCut.resize( dataSize );
	
	audioData.linearEQIntercept	= 0.9;
	audioData.linearEQSlope		= 1.0;
}

void AudioAbstract :: updateAudioData ( AudioData &audioData, float *dataNew )
{
	audioData.data.clear();
	audioData.data.resize( audioData.size );
	
	copy( dataNew, dataNew + audioData.size, audioData.data.begin() );
	
	audioData.peakAverage = 0;
	
	for( int i=0; i<audioData.size; i++ )
	{
		float p = i / (float)( audioData.size - 1 );
		
		float dataVal;
		dataVal  = audioData.data[ i ];												// use magnitude for fft data.
		dataVal *= audioData.linearEQIntercept + p * audioData.linearEQSlope;		// scale value.
		
		float dataMaxVal;
		dataMaxVal = audioData.dataMax[ i ];
		
		if( dataMaxVal < dataVal )
		{
			dataMaxVal = dataVal;
		}
		
		float dataNormVal;
		dataNormVal = 0;
		if( dataMaxVal > 0 )
			dataNormVal = dataVal / dataMaxVal;						// normalise data between 0 and 1.
		
		dataMaxVal *= audioData.maxDecay;							// decay the max value.
		
		audioData.dataNorm[ i ]		= dataNormVal;
		
		audioData.dataMax[ i ]		= dataMaxVal;
		
		float dataPeakVal;
		dataPeakVal  = audioData.dataPeak[ i ];
		dataPeakVal *= audioData.peakDecay;							// decay peak value.
		
		if( dataPeakVal < dataNormVal )								// check if new peak.
			dataPeakVal = dataNormVal;
		
		audioData.dataPeak[ i ]		= dataPeakVal;
		
		audioData.peakAverage		+= dataPeakVal;					// sum of all peaks.
		
		int dataCutVal;												// switch data (on/off).
		if( dataPeakVal < audioData.cutThreshold )
			dataCutVal = 1;
		else
			dataCutVal = 0;
		
		audioData.dataCut[ i ]		= dataCutVal;
	}
	
	audioData.peakAverage /= audioData.size;
}

void AudioAbstract :: mirrorAudioData ( AudioData &audioData )
{
	audioData.peakAverage = 0;
	
	int audioDataSizeHalf;
	audioDataSizeHalf = (int)( audioData.size * 0.5 );
	
	for( int i=0; i<audioDataSizeHalf; i++ )
	{
		int j = audioData.size - 1;
		
		audioData.dataNorm[ j - i ] = audioData.dataNorm[ i ];
		audioData.dataMax[ j - i ]	= audioData.dataMax[ i ];
		audioData.dataPeak[ j - i ]	= audioData.dataPeak[ i ];
		audioData.dataCut[ j - i ]	= audioData.dataCut[ i ];
	}
	
	audioData.peakAverage /= audioDataSizeHalf;
}

void AudioAbstract :: resetAudioData ( AudioData &audioData )
{
	audioData.data.resize( audioData.size );
	audioData.dataNorm.resize( audioData.size );
	audioData.dataMax.resize( audioData.size );
	audioData.dataPeak.resize( audioData.size );
	audioData.dataCut.resize( audioData.size );
}

//////////////////////////////////////////////////////
//	SETTERS / GETTERS.
//////////////////////////////////////////////////////

void AudioAbstract :: setNoOfBands( int value )
{
	int audioNoOfBandsNew;
	audioNoOfBandsNew	= ofNextPow2( value );
	
	if( audioNoOfBands == audioNoOfBandsNew )
		return;
	
	audioNoOfBands		= audioNoOfBandsNew;
	audioNoOfBandsHalf	= (int)( audioNoOfBands * 0.5 );
	
	resetFFT();
	initFFT();
	
	resetAudioData( rawData );
	resetAudioData( fftData );
}

int AudioAbstract :: getNoOfBands()
{
	return audioNoOfBands;
}

void AudioAbstract :: setThreshold( float value )
{
	float cutThreshold;
	cutThreshold = cutThreshold = MIN( 1, MAX( 0, value ) );
	
	rawData.cutThreshold = cutThreshold;
	fftData.cutThreshold = cutThreshold;
}

float AudioAbstract :: getThreshold()
{
	return fftData.cutThreshold;
}

float AudioAbstract :: getAveragePeak()
{
	return fftData.peakAverage;
}

void AudioAbstract :: setPeakDecay ( float value )
{
	value = MIN( 1, MAX( 0, value ) );
	
	rawData.peakDecay = value;
	fftData.peakDecay = value;
}

float AudioAbstract :: getPeakDecay ()
{
	return fftData.peakDecay;
}

void AudioAbstract :: setMaxDecay ( float value )
{
	value = MIN( 1, MAX( 0, value ) );
	
	rawData.maxDecay = value;
	fftData.maxDecay = value;
}

float AudioAbstract :: getMaxDecay ()
{
	return fftData.maxDecay;
}

void AudioAbstract :: setMirrorData ( bool value )
{
	bMirrorData = value;
}

void AudioAbstract :: setUseFftData ( bool value )
{
	bUseFftData = value;
}

//////////////////////////////////////////////////////
//	GET VARIOUS DATA SAMPLES.
//////////////////////////////////////////////////////

void AudioAbstract :: getFftData ( float *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBands - 1 ) );
		float v		= fftData.dataNorm[ j ];
		data[ i ]	= v;
	}
}

void AudioAbstract :: getFftPeakData ( float *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBands - 1 ) );
		float v		= fftData.dataPeak[ j ];
		data[ i ]	= v;
	}
}

void AudioAbstract :: getGlitchData( int *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBands - 1 ) );
		float v		= fftData.dataCut[ j ];
		data[ i ]	= v;
	}
}

//////////////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////////////

void AudioAbstract :: draw( int x, int y, int w, int h ) 
{
	glPushMatrix();
	glTranslatef( x, y, 0 );
	
		draw( w, h );
	
	glPopMatrix();
}

void AudioAbstract :: draw( int width, int height )
{
	renderSingleBandWidth = width / (float)audioNoOfBands;
	
	if( bUseFftData )
	{
		drawData( fftData, width, height );
	}
	else
	{
		drawData( rawData, width, height );
	}
}

void AudioAbstract :: drawData ( const AudioData &audioData, int width, int height )
{
	drawBg( audioData, width, height );
	drawGlitchData( audioData, width, height );
//	drawFftData( audioData, width, height );			// this is audio data before its been normalised, good for debugging.
	drawFftNormData( audioData, width, height );
	drawFftPeakData( audioData, width, height );
	drawThresholdLine( audioData, width, height );
}

void AudioAbstract :: drawBg ( const AudioData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	ofFill();
	
	ofSetColor( 113, 113, 113 );
	ofRect( 0, 0, w + bx * 2, h + by * 2 );
	
	ofSetColor( 232, 232, 232 );
	ofRect( bx, by, w, h );
}

void AudioAbstract :: drawGlitchData ( const AudioData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		ofFill();
		ofSetColor( 0, 255, 255 );
		ofRect
		(
			i * renderSingleBandWidth + bx,
			h + by,
			renderSingleBandWidth,
			-audioData.dataCut[ i ] * h
		);
	}
}

void AudioAbstract :: drawFftData ( const AudioData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		ofFill();
		ofSetColor( 50, 50, 50 );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.data[ i ] * h );
		
		ofNoFill();
		ofSetColor( 232, 232, 232 );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.data[ i ] * h );
	}
}

void AudioAbstract :: drawFftNormData ( const AudioData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		ofFill();
		ofSetColor( 50, 50, 50 );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.dataNorm[ i ] * h );
		
		ofNoFill();
		ofSetColor( 232, 232, 232 );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.dataNorm[ i ] * h );
	}
}

void AudioAbstract :: drawFftPeakData ( const AudioData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBands; i++ )
	{
		ofFill();
		ofSetColor( 0, 0, 255 );
		ofRect( i * renderSingleBandWidth + bx, ( 1 - audioData.dataPeak[ i ] ) * h + by, renderSingleBandWidth, 2 );
	}
}

void AudioAbstract :: drawThresholdLine ( const AudioData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	ofNoFill();
	ofSetColor( 255, 0, 255 );
	ofLine( bx, ( 1 - audioData.cutThreshold ) * h + by, w + bx, ( 1 - audioData.cutThreshold ) * h + by );
}