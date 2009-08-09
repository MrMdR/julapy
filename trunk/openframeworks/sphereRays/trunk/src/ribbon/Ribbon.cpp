/*
 *  Ribbon.cpp
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 29/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Ribbon.h"

Ribbon :: Ribbon()
{
	progressIn		= 0;
	progressOut		= 0;
	progressStep	= 0.05;
	curveWidth		= 0.05;
	curveHeight		= 0.4;
	playedIn		= false;
	playedOut		= false;
	
	detail				= 100;
	lineStrip			= new GLfloat[ detail * 3 ];
	lineColor			= new GLfloat[ detail * 4 ];
	triangleStrip		= new GLfloat[ detail * 6 ];
	triangleStripNormal	= new GLfloat[ detail * 6 ];
	triangleStripColor	= new GLfloat[ detail * 8 ];
}

Ribbon :: ~Ribbon()
{
	delete[] lineStrip;
	delete[] lineColor;
	delete[] triangleStrip;
	delete[] triangleStripNormal;
	delete[] triangleStripColor;
}

void Ribbon :: set( float *points )
{
	for( int i=0; i<4; i++ )
	{
		this->points[ i ] = points[ i ];
	}
	
	calcLine();
}


void Ribbon :: setDetail( int value )
{
	detail = value;
	
	calcLine();
}

void Ribbon :: calcLine()
{
	//
}

void Ribbon :: reset()
{
	progressIn	= 0;
	progressOut	= 0;
	
	playedIn	= false;
	playedOut	= false;
}

////////////////////////////////////////////////////////////
//	SETTERS.
////////////////////////////////////////////////////////////

void Ribbon :: setProgressIn( float value )
{
	progressIn = value;
}

void Ribbon :: setProgressOut( float value )
{
	progressOut = value;
}

void Ribbon :: setProgressStep( float value )
{
	progressStep = value;
}

void Ribbon :: setCurveWidth( float value )
{
	curveWidth = value;
}

void Ribbon :: setCurveHeight( float value )
{
	curveHeight = value;
}

////////////////////////////////////////////////////////////
//	PROGRESS.
////////////////////////////////////////////////////////////

void Ribbon :: playInStep ()
{
	progressOut += progressStep;
	if( progressOut >= 1 )
	{
		progressOut = 1;
		playedIn	= true;
	}
}

void Ribbon :: playOutStep ()
{
	progressIn += progressStep;
	if( progressIn >= 1 )
	{
		progressIn	= 1;
		playedOut	= true;
	}
}

bool Ribbon :: isPlayedIn()
{
	return playedIn;
}

bool Ribbon :: isPlayedOut()
{
	return playedOut;
}

////////////////////////////////////////////////////////////
//	DRAW.
////////////////////////////////////////////////////////////

void Ribbon :: drawLine()
{
	int i = (int)( progressIn * ( detail - 1 ) );
	int j = (int)( progressOut * ( detail - 1 ) );
	int c = j - i;
	
	drawLineStrip( lineStrip, lineColor, i, c );
}

void Ribbon :: drawFill()
{
	int i = (int)( progressIn * ( detail - 1 ) ) * 2;
	int j = (int)( progressOut * ( detail - 1 ) ) * 2;
	int c = j - i;
	
	drawTriangleStrip( triangleStrip, triangleStripNormal, triangleStripColor, i, c );
}

void Ribbon :: drawLineStrip( GLfloat *vertex, GLfloat *color, int first, int count )
{
	glVertexPointer( 3, GL_FLOAT, 0, &vertex[ 0 ] );
	glColorPointer( 4, GL_FLOAT, 0, &color[ 0 ] );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glDrawArrays( GL_LINE_STRIP, first, count );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}

void Ribbon :: drawTriangleStrip( GLfloat *vertex, GLfloat *normal, GLfloat *color, int first, int count )
{
	glShadeModel( GL_SMOOTH );
	
	glVertexPointer( 3, GL_FLOAT, 0, &vertex[ 0 ] );
	glNormalPointer( GL_FLOAT, 0, &normal[ 0 ] );
	glColorPointer( 4, GL_FLOAT, 0, &color[ 0 ] );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glDrawArrays( GL_TRIANGLE_STRIP, first, count );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}
