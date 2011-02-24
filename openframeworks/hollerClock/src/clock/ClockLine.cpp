/*
 *  ClockLine.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 3/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ClockLine.h"

ClockLine :: ClockLine ()
{
	tex			= NULL;
	angle		= 0;
	scale		= ofRandom( 0.8, 1.0 );
	size.width	= 0;
	size.height	= 0;
	length		= 0;
	
	pointsNum	= 3;
	pointsPos	= new float[ pointsNum ];
	pointsVel	= new float[ pointsNum ];
	
	for( int i=0; i<pointsNum; i++ )
	{
		pointsPos[ i ] = ofRandom( 0.0,   1.0   );
		pointsVel[ i ] = ofRandom( 0.001, 0.005 );
	}
}

ClockLine :: ~ClockLine ()
{

}

void ClockLine :: setTexture ( ofTexture* tex )
{
	this->tex = tex;
}

void ClockLine :: setup ()
{
	float half_w	= size.width  * 0.5;
	float half_h	= size.height * 0.5;
	float diag_len	= sqrt( half_w * half_w + half_h * half_h );
	float diag_rad	= acos( half_h / diag_len );
	float diag_ang	= diag_rad * RAD_TO_DEG;
	
	float a;
	
	if( angle < diag_ang )
	{
		a		= angle;
		length	= half_h / cos( a * DEG_TO_RAD );
	}
	else if( angle < 90 )
	{
		a		= 90 - angle;
		length	= half_w / cos( a * DEG_TO_RAD );
	}
	else if( angle < 180 - diag_ang )
	{
		a		= angle - 90;
		length	= half_w / cos( a * DEG_TO_RAD );
	}
	else if( angle < 180 )
	{
		a		= 180 - angle;
		length	= half_h / cos( a * DEG_TO_RAD );
	}
	else if( angle < 180 + diag_ang )
	{
		a		= angle - 180;
		length	= half_h / cos( a * DEG_TO_RAD );
	}
	else if( angle < 270 )
	{
		a		= 270 - angle;
		length	= half_w / cos( a * DEG_TO_RAD );
	}
	else if( angle < 360 - diag_ang )
	{
		a		= angle - 270;
		length	= half_w / cos( a * DEG_TO_RAD );
	}
	else if( angle < 360 )
	{
		a		= 360 - angle;
		length	= half_h / cos( a * DEG_TO_RAD );
	}
	
	linePoints[ 0 ] = 0;
	linePoints[ 1 ] = 0;
	linePoints[ 2 ] = length;
	linePoints[ 3 ] = 0;
	
	lineColors[ 0 ] = 0.0;
	lineColors[ 1 ] = 0.0;
	lineColors[ 2 ] = 0.0;
	lineColors[ 3 ] = 0.0;
	lineColors[ 4 ] = 0.0;
	lineColors[ 5 ] = 0.0;
	lineColors[ 6 ] = 0.0;
	lineColors[ 7 ] = 0.1;
	
	float w = 5;
	float h = 3;
	
	rectPoints[ 0 ] = -w/2;
	rectPoints[ 1 ] = -h/2;
	rectPoints[ 2 ] = w/2;
	rectPoints[ 3 ] = -h/2;
	rectPoints[ 4 ] = w/2;
	rectPoints[ 5 ] = h/2;
	rectPoints[ 6 ] = -w/2;
	rectPoints[ 7 ] = h/2;
}

void ClockLine :: draw ( float w, float h )
{
	if( tex == NULL )
	{
		drawLine( w, h );
	}
	else 
	{
		drawTexture( w, h );
	}

}

void ClockLine :: drawLine ( float w, float h )
{
	glPushMatrix();
	glRotatef( angle - 90, 0, 0, 1 );
	
	//-- line.
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glColorPointer( 4, GL_FLOAT, 0, &lineColors[ 0 ] );
	glVertexPointer( 2, GL_FLOAT, 0, &linePoints[ 0 ] );
	
	glDrawArrays( GL_LINES, 0, 2 );
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState( GL_COLOR_ARRAY );

	//-- rect.

	for( int i=0; i<pointsNum; i++ )
	{
		glColor4f( 0, 0, 0, pointsPos[ i ] * 0.15 );
		
		ofFill();
		ofRect( ( pointsPos[ i ] * length ) + 5, -1.5, 5, 3 );
		
		pointsPos[ i ] += pointsVel[ i ];
		
		if( pointsPos[ i ] > 1.0 )
			pointsPos[ i ] = 0;
	}
	
	glPopMatrix();
}

void ClockLine :: drawTexture ( float w, float h )
{
	ofSetColor( 255, 255, 255, 30 );
	
	glPushMatrix();
	glTranslatef( 0, -tex->getHeight() * 0.5, 0 );
	glScalef( scale, scale, 0 );
	glRotatef( angle, 0, 0, 1 );
		tex->draw( 0, 0 );
	glPopMatrix();
}
