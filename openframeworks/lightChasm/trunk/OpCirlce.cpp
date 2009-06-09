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

void OpCirlce :: update()
{

}

void OpCirlce :: draw()
{
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
				ofSetColor( 0, 0, 0 );
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
	
	glPopMatrix();
}