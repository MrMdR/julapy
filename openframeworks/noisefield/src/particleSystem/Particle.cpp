/*
 *  Particle.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"

Particle :: Particle( int x, int y, double dx, double dy )
{
	_x	= x;
	_y	= y;
	_dx	= dx;
	_dy	= dy;
}

Particle :: ~Particle()
{
	
}

void Particle :: addForce( double fx, double fy )
{
	_dxt += fx;
	_dyt += fy;
}

void Particle :: update()
{
	_dx = _dxt;
	_dy = _dyt;
	
	_x += _dx;
	_y += _dy;
	
	_dxt = 0;
	_dyt = 0;
}

void Particle :: draw()
{
	ofFill();
	ofSetColor( 255, 0, 0, 255 );
	ofCircle( _x, _y, 4.0f );
}