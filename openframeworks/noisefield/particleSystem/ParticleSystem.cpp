/*
 *  ParticleSystem.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleSystem.h"

ParticleSystem :: ParticleSystem( int w, int h )
{
	int i;
	
	width				= w;
	height				= h;
	particlesTotal		= 4000;
	particleCount		= 0;
	particleBoundsGap	= 50;
	
	for( i=0; i<particlesTotal; i++ )
	{
		addParticle
		(
			ofRandom( 0, width ),
			ofRandom( 0, height ),
			0,
			0
		);
	}
}

ParticleSystem :: ~ParticleSystem()
{
	//
}

void ParticleSystem :: addParticle( int x, int y, double dx, double dy )
{
	particles.push_back
	(
		Particle
		(
			ofRandom( 0, width ),
			ofRandom( 0, height ),
			0,
			0
		)
	);
}

Boolean ParticleSystem :: hasNextParticle()
{
	if( particleCount < particles.size() )
	{
		++particleCount;
		
		return true;
	}
	else
	{
		particleCount = 0;
		
		return false;
	}
}

Particle* ParticleSystem :: getParticle()
{
	return &particles[ particleCount - 1 ];
}

void ParticleSystem :: update()
{
	int i;
	
	for( i=0; i<particles.size(); i++ )
	{
		particles[ i ].update();
	}
	
	checkBounds();
}

void ParticleSystem :: checkBounds ()
{
	int i;
	Boolean lb;
	Boolean	tb;
	Boolean bb;
	Boolean rb;
	
	for( i=0; i<particles.size(); i++ )
	{
		lb	= ( particles[ i ]._x < -particleBoundsGap );
		rb	= ( particles[ i ]._x > width + particleBoundsGap );
		tb	= ( particles[ i ]._y < -particleBoundsGap );
		bb	= ( particles[ i ]._y > height + particleBoundsGap );
		
		if( lb || rb || tb || bb )
		{
			if( ofRandom( 0, 1 ) > 0.5 )
			{
				if( ofRandom( 0, 1 ) > 0.5 )
				{
					particles[ i ]._x = -particleBoundsGap * 0.5f;
					particles[ i ]._y = ofRandom( 0, height );
				}
				else
				{
					particles[ i ]._x = width + particleBoundsGap * 0.5f;
					particles[ i ]._y = ofRandom( 0, height );
				}
			}
			else
			{
				if( ofRandom( 0, 1 ) )
				{
					particles[ i ]._x = ofRandom( 0, width );
					particles[ i ]._y = -particleBoundsGap * 0.5f;
				}
				else
				{
					particles[ i ]._x = ofRandom( 0, width );
					particles[ i ]._y = width + particleBoundsGap * 0.5f;
				}
			}
		}
	}
}

void ParticleSystem :: draw()
{
	int i;
	
	for( i=0; i<particles.size(); i++ )
	{
		particles[ i ].draw();
	}
}