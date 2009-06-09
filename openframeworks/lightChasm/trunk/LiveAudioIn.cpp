/*
 *  LiveAudioIn.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 9/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "LiveAudioIn.h"

LiveAudioIn :: LiveAudioIn()
{

}

LiveAudioIn :: ~LiveAudioIn()
{

}

void LiveAudioIn :: init()
{
	averagesPerOctave	= 2;
	averagesTotal		= averagesPerOctave * 9;
	
	fftAnalyzer.setup( 44100, BUFFER_SIZE / 2, averagesPerOctave );

	left		= new float[ BUFFER_SIZE ];
	right		= new float[ BUFFER_SIZE ];
	averages	= new float[ averagesTotal ];
	peaks		= new float[ averagesTotal ];
	
	fftAnalyzer.peakHoldTime		= 15;		// hold longer
	fftAnalyzer.peakDecayRate		= 0.95f;	// decay slower
	fftAnalyzer.linearEQIntercept	= 0.9f;		// reduced gain at lowest frequency
	fftAnalyzer.linearEQSlope		= 0.01f;	// increasing gain at higher frequencies
}

void LiveAudioIn :: update()
{
	myfft.powerSpectrum
	(
		0,
		(int)( BUFFER_SIZE / 2.0F ),
		left,
		BUFFER_SIZE,
		&magnitude[ 0 ],
		&phase[ 0 ],
		&power[ 0 ],
		&avgPower
	);
	
	for( int i=0; i<(int)( BUFFER_SIZE / 2.0f ); i++ )
	{
		freq[ i ] = magnitude[ i ];
	}
	
	fftAnalyzer.calculate( freq );
	
	for( int i=0; i<averagesTotal; i++ )
	{
		averages[ i ]	= fftAnalyzer.averages[ i ];
		peaks[ i ]		= fftAnalyzer.peaks[ i ];
	}
}

void LiveAudioIn :: draw()
{
	ofFill();
	ofSetColor( 0xAAAAAA );
	for( int i=0; i<averagesTotal; i++ )
	{
		ofRect
		(
			ofGetWidth() - ( averagesTotal + 1 ) * 20 + ( i * 20 ),
			ofGetHeight(),
			20,
			-averages[ i ] * 6
		);
	}
	
	ofSetColor( 0xFF0000 );
	for( int i=0; i<fftAnalyzer.nAverages; i++ )
	{
		ofRect
		(
			ofGetWidth() - ( averagesTotal + 1 ) * 20 + ( i * 20 ),
			ofGetHeight() - peaks[ i ] * 6,
			20,
			-4
		);
	}
}
