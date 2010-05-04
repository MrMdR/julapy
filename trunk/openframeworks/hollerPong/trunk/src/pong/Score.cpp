/*
 *  Score.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 6/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Score.h"

void Score :: init()
{
	font.loadFont( "fonts/mono0755.ttf", 56 );
	
	reset();
}

void Score :: addToPlayerOne( int p )
{
	points[ 0 ] += p;
}

void Score :: addToPlayerTwo( int p )
{
	points[ 1 ] += p;
}

void Score :: reset ()
{
	points[ 0 ] = 0;
	points[ 1 ] = 0;
}

void Score :: update ()
{
	//
}

void Score :: draw ()
{
	int c;
	c = (int)( ofGetWidth() * 0.5 );
	
	string p1s;
	p1s  = "";
	p1s += ( points[ 0 ] < 10 ) ? "0" : "";
	p1s += ofToString( points[ 0 ], 0 );

	string p2s;
	p2s  = "";
	p2s += ( points[ 1 ] < 10 ) ? "0" : "";
	p2s += ofToString( points[ 1 ], 0 );
	
	ofSetColor( 0xFFFFFF );
	
	int border;
	border = PONG_BORDER_THICKNESS;
	
	font.drawString( p1s, c - 140, 80 + border );
	font.drawString( p2s, c +  30, 80 + border );
}