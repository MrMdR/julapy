/*
 *  Particle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/03/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"

Particle :: Particle( PixelFlow* pfImage, PixelFlow* pfTrace )
{
	this->pfImage = pfImage;
	this->pfTrace = pfTrace;
	
	pfSampleRangeX	= 5;
	pfSampleRangeY	= 5;
	
	imgVecScale = 100;
	trcVecScale = 100;
	
	//---
	
	line_ver_array = new GLfloat[ PARTICLE_MAX_LENGTH * 3 ];
	line_col_array = new GLfloat[ PARTICLE_MAX_LENGTH * 4 ];
	line_ind_total = 0;

	strip_ver_array = new GLfloat[ PARTICLE_MAX_LENGTH * 6 ];
	strip_col_array = new GLfloat[ PARTICLE_MAX_LENGTH * 8 ];
	strip_ind_total = 0;
	
	//---
	
	size		= 2;
	sizeHalf	= size * 0.5;
	
	lineAlpha	= 1.0;
	
	stripWidth	= 0.1;
	
	colorEase	= 0.1;
	
	bEnableImageForce	= true;
	bEnableTraceForce	= true;
	bMarkAsTestParticle	= false;
	bUseImageColour		= true;
}

Particle :: ~Particle()
{
	pfImage = NULL;
	pfTrace = NULL;
}

//////////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////////

void Particle :: setInitialPosition	( float x, float y )
{
	posVec.x = x;
	posVec.y = y;
	
	posPrevVec.x = x;
	posPrevVec.y = y;
	
	currentColor = pfImage->getColourAt( posVec );
	
	addToLineVertexArray( posVec, currentColor );
	addToStripVertexArray( posVec, posVec, currentColor, currentColor );
}

void Particle :: setInitialVelocity	( float x, float y )
{
	velVec.x = x;
	velVec.y = y;
}

void Particle :: setPixelRange ( int x, int y )
{
	pfSampleRangeX = x;
	pfSampleRangeY = y;
}

void Particle :: setup ()
{
	
}

//////////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////////

void Particle :: update ()
{
	posPrevVec.x = posVec.x;
	posPrevVec.y = posVec.y;
	
	//--
	
	ofxVec2f totalVec;
	
	imgVec.set( 0, 0 );
	trcVec.set( 0, 0 );
	
	if( bEnableImageForce )
	{
		imgVec = pfImage->getVectorAt( posVec, pfSampleRangeX, pfSampleRangeY );
		imgVec *= imgVecScale;
	
		totalVec += imgVec;
	}
	
	if( bEnableTraceForce )
	{
		trcVec = pfTrace->getVectorAt( posVec, pfSampleRangeX, pfSampleRangeY );
		trcVec *= trcVecScale * -1;
		
		totalVec += trcVec;
	}
	
	totalVec.limit( 2 );
	
	float ease = 0.2;
	
	velVec.x += ( totalVec.x - velVec.x ) * ease;
	velVec.y += ( totalVec.y - velVec.y ) * ease;
	
	posVec.x += velVec.x;
	posVec.y += velVec.y;
	
	//---
	
	if( bUseImageColour )
	{
		ofColor c = pfImage->getColourAt( posVec );
		
		currentColor.r += ( c.r - currentColor.r ) * colorEase;
		currentColor.g += ( c.g - currentColor.g ) * colorEase;
		currentColor.b += ( c.b - currentColor.b ) * colorEase;
		currentColor.a += ( c.a - currentColor.a ) * colorEase;
	}
	else
	{
		currentColor.r = 255;
		currentColor.g = 255;
		currentColor.b = 255;
		currentColor.a = 128;
	}

	addToLineVertexArray( posVec, currentColor );
	addToStrip();
}

//////////////////////////////////////////////////
//	VERTEX ARRAYS.
//////////////////////////////////////////////////

void Particle :: addToStrip ()
{
	ofxVec2f v1 = posVec - posPrevVec;
	v1.perpendicular();
	v1.normalize();
	v1 *= stripWidth;
	
	stripPoint0.x = posVec.x + v1.x;
	stripPoint0.y = posVec.y + v1.y;
	stripPoint0.z = 0;
	
	stripPoint1.x = posVec.x - v1.x;
	stripPoint1.y = posVec.y - v1.y;
	stripPoint1.z = 0;
	
	addToStripVertexArray( stripPoint0, stripPoint1, currentColor, currentColor );
}

void Particle :: addToLineVertexArray ( const ofPoint& p, const ofColor& c )
{
	if( line_ind_total >= PARTICLE_MAX_LENGTH )
	{
		cout << "particle " << pid << ", PARTICLE_MAX_LENGTH exceeded.";
		return;
	}
	
	int i = line_ind_total * 3;
	int j = line_ind_total * 4;
	
	line_ver_array[ i + 0 ] = p.x;
	line_ver_array[ i + 1 ] = p.y;
	line_ver_array[ i + 2 ] = 0;
	
	line_col_array[ j + 0 ] = c.r / 255.0;
	line_col_array[ j + 1 ] = c.g / 255.0;
	line_col_array[ j + 2 ] = c.b / 255.0;
	line_col_array[ j + 3 ] = c.a / 255.0;
	
	line_ind_total += 1;
}

void Particle :: addToStripVertexArray ( const ofPoint& p1, const ofPoint& p2, const ofColor& c1, const ofColor& c2 )
{
	if( strip_ind_total >= ( PARTICLE_MAX_LENGTH * 2 ) )
	{
		cout << "particle " << pid << ", PARTICLE_MAX_LENGTH exceeded.";
		return;
	}
	
	int i = strip_ind_total * 3;
	int j = strip_ind_total * 4;
	
	strip_ver_array[ i + 0 ] = p1.x;
	strip_ver_array[ i + 1 ] = p1.y;
	strip_ver_array[ i + 2 ] = 0;
	strip_ver_array[ i + 3 ] = p2.x;
	strip_ver_array[ i + 4 ] = p2.y;
	strip_ver_array[ i + 5 ] = 0;
	
	strip_col_array[ j + 0 ] = c1.r / 255.0;
	strip_col_array[ j + 1 ] = c1.g / 255.0;
	strip_col_array[ j + 2 ] = c1.b / 255.0;
	strip_col_array[ j + 3 ] = c1.a / 255.0;
	strip_col_array[ j + 4 ] = c2.r / 255.0;
	strip_col_array[ j + 5 ] = c2.g / 255.0;
	strip_col_array[ j + 6 ] = c2.b / 255.0;
	strip_col_array[ j + 7 ] = c2.a / 255.0;
	
	strip_ind_total += 2;
}

//////////////////////////////////////////////////
//	DRAW
//////////////////////////////////////////////////

void Particle :: drawHead ()
{
	glColor4f( 1.0, 0.0, 0.0, 1.0 );
	
	if( bMarkAsTestParticle )
	{
		glColor4f( 0.0, 1.0, 0.0, 1.0 );
	}
	
	ofRect( posVec.x - sizeHalf, posVec.y - sizeHalf, size, size );
}

void Particle :: drawLine ()
{
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer( 4, GL_FLOAT, 0, line_col_array );
	
	glEnableClientState( GL_VERTEX_ARRAY );		
	glVertexPointer( 3, GL_FLOAT, 0, line_ver_array );
	
	glDrawArrays( GL_LINE_STRIP, 0, line_ind_total );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Particle :: drawStrip ()
{
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer( 4, GL_FLOAT, 0, strip_col_array );
	
	glEnableClientState( GL_VERTEX_ARRAY );		
	glVertexPointer( 3, GL_FLOAT, 0, strip_ver_array );
	
	glDrawArrays( GL_QUAD_STRIP, 0, strip_ind_total );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Particle :: drawTrace ()
{
	bool b1 = ( posVec.x == posPrevVec.x );
	bool b2 = ( posVec.y == posPrevVec.y );
	
	if( b1 && b2  )
		return;
	
	glColor4f
	( 
		currentColor.r / 255.0,
		currentColor.g / 255.0,
		currentColor.b / 255.0,
		currentColor.a / 255.0
	);
	
	ofLine( posPrevVec.x, posPrevVec.y, posVec.x, posVec.y );
}
