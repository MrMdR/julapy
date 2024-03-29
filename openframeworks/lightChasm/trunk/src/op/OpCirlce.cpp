/*
 *  OpCirlce.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 8/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpCirlce.h"

OpCirlce :: OpCirlce()
{
	x			= 0;
	y			= 0;
	radius		= 100;
	
	setCirlceResolution( 4 );
	setInverse( false );
	setRotation( 0 );
	setAudioInValue( 0 );
	setRgbScale( 0, 0, 0 );
	
	fbo.allocate( 640, 480, true );
}

OpCirlce :: ~OpCirlce()
{

}

void OpCirlce :: setCirlceResolution( int res )
{
	resolution = MIN( MAX( MIN_CIRCLE_PTS, res ), MAX_CIRCLE_PTS );
	
	float angle			= 0.0f;
	float angleAdder	= M_TWO_PI / (float)resolution;
	int k				= 0;
		
	for( int i=0; i<resolution+1; i++ )
	{
		circlePts[ k ]		= cos( angle );
		circlePts[ k + 1 ]	= sin( angle );
		angle	+= angleAdder;
		k		+= 2;
	}
}

void OpCirlce :: setInverse ()
{
	inverse = !inverse;
}

void OpCirlce :: setInverse ( bool b )
{
	inverse = b;
}

void OpCirlce :: setRotation( float degrees )
{
	rotation = degrees;
}

void OpCirlce :: setAudioInValue( float value )
{
	if( audioInValue < value )
	{
		audioInValue = value;
	}
	
	audioInValue *= 0.93;
	
//	audioInValue += ( value - audioInValue ) * 0.1;
}

void OpCirlce :: setRgbScale( float r, float g, float b )
{
	rScale = r;
	gScale = g;
	bScale = b;
	
//	printf( "rScale: %4.2f gScale: %4.2f bScale: %4.2f \n", rScale, gScale, bScale );
}

void OpCirlce :: update()
{

}

void OpCirlce :: drawToFBO()
{
	fbo.swapIn();
	fbo.setupScreenForMe();
	
	draw();
	
	fbo.swapOut();
	fbo.setupScreenForThem();
}

void OpCirlce :: drawFBOToScreen()
{
	ofSetColor( 255, 255, 255 );
	fbo.draw( 0, 0 );
}

void OpCirlce :: draw()
{
	ofEnableSmoothing();
	
	int k = 0;

	glPushMatrix();
	glTranslatef( x, y, 0 );
	glRotatef( rotation, 0, 0, 1 );
	
	for( int i=0; i<resolution; i++ )
	{
		if( i % 2 == 0 )
		{
			if( inverse )
			{
				ofSetColor( 255, 255, 255 );
			}
			else
			{
				ofSetColor( (int)( rScale * audioInValue * 255 ), (int)( gScale * audioInValue * 255 ), (int)( bScale * audioInValue * 255 ) );
			}
		}
		else
		{
			if( inverse )
			{
				ofSetColor( 0, 0, 0 );
			}
			else
			{
				ofSetColor( 255, 255, 255 );
			}
		}
		
		if( resolution > 3 )
		{
			ofFill();
			ofTriangle
			(
				0,
				0,
				circlePts[ k + 0 ] * radius,
				circlePts[ k + 1 ] * radius,
				circlePts[ k + 2 ] * radius,
				circlePts[ k + 3 ] * radius
			);
		
			ofNoFill();
			ofTriangle
			(
				0,
				0,
				circlePts[ k + 0 ] * radius,
				circlePts[ k + 1 ] * radius,
				circlePts[ k + 2 ] * radius,
				circlePts[ k + 3 ] * radius
			);
			
			k += 2;
		}
	}
	
	glPopMatrix();
	
	ofDisableSmoothing();
}