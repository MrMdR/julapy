/*
 *  OpParticleRain.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 25/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OpParticleRain.h"

OpParticleRain :: OpParticleRain()
{

}

OpParticleRain :: ~OpParticleRain()
{

}

void OpParticleRain :: init( int w, int h )
{
	width	= w;
	height	= h;
	
	setMinParticleSize( 1 );
	setMaxParticleSize( 7 );
	setMinParticleVel( 1.5 );
	setMaxParticleVel( 4.0 );
	setMakeRate( 0.2 );
	
	fbo.allocate( w, h, true );
}

void OpParticleRain :: update()
{
	if( ofRandom( 0.0, 1.0 ) < makeRate )
	{
		int makeNo = (int)( ofRandom( 0, makeRate ) * 30 ) + 1;
		
		for( int i=0; i<makeNo; i++ )
		{
			float size		= ofRandom( minParticleSize, maxParticleSize );
			float sizeScale	= ( size - minParticleSize ) / ( maxParticleSize - minParticleSize );
			
			particles.push_back( OpParticleRainDrop() );
			particles.back().alive			= true;
			particles.back().size			= size;
			particles.back().wobbleMag		= ofRandom( 1, 10 );
			particles.back().wobbleAng		= ofRandom( 0, 2 * PI );
			particles.back().wobbleAngInc	= 2 * PI * 0.05 * MAX( 0.1, sizeScale );
			particles.back().loc[ 0 ] = ofRandom( 0, width );
			particles.back().loc[ 1 ] = height + size;
			particles.back().vel[ 0 ] = 0;
			particles.back().vel[ 1 ] = ( minParticleVel + sizeScale * maxParticleVel ) * ( ofRandom( 0.0, 0.15 ) + 1 );
		}
	}
	
	for( int i=0; i<particles.size(); i++ )
	{
		particles.at( i ).loc[ 0 ]	-= particles.at( i ).vel[ 0 ];
		particles.at( i ).loc[ 1 ]	-= particles.at( i ).vel[ 1 ];
		particles.at( i ).wobbleAng += particles.at( i ).wobbleAngInc;
		
		if( particles.at( i ).loc[ 1 ] < -particles.at( i ).size )
		{
			particles.at( i ).alive = false;
		}
	}
	
	for( int i=0; i<particles.size(); i++ )
	{
		if( !particles.at( i ).alive )
		{
			particles.erase( particles.begin() + i );
			--i;
		}
	}
}

void OpParticleRain :: drawToFBO()
{
	fbo.swapIn();
	fbo.setupScreenForMe();

	draw();

	fbo.swapOut();
	fbo.setupScreenForThem();
}

void OpParticleRain :: drawFBOToScreen()
{
	ofSetColor( 255, 255, 255 );
	fbo.draw( 0, 0 );
}

void OpParticleRain :: draw()
{
	ofEnableSmoothing();
	
	ofSetColor( 0, 0, 0 );
	
	for( int i=0; i<particles.size(); i++ )
	{
		float x = particles.at( i ).loc[ 0 ];
		float y = particles.at( i ).loc[ 1 ];
		float s = particles.at( i ).size;
		
		x += particles.at( i ).wobbleMag * sin( particles.at( i ).wobbleAng );
		
		ofNoFill();
		ofCircle( x, y, s );
		ofFill();
		ofCircle( x, y, s );
	}
	
	ofDisableSmoothing();
}

unsigned char* OpParticleRain :: getPixels()
{
	return new unsigned char[ 1 ];
//	return fbo.getTextureData();
}

void OpParticleRain :: setMinParticleSize( float value )
{
	minParticleSize = value;
}

void OpParticleRain :: setMaxParticleSize( float value )
{
	maxParticleSize = value;
}

void OpParticleRain :: setMinParticleVel( float value )
{
	minParticleVel = value;
}

void OpParticleRain :: setMaxParticleVel( float value )
{
	maxParticleVel = value;
}

void OpParticleRain :: setMakeRate( float value )
{
	makeRate = value;
	if( makeRate < 0 )
		makeRate = 0;
	if( makeRate > 1 )
		makeRate = 1;
}