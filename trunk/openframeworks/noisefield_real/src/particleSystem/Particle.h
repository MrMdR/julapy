/*
 *  Particle.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class Particle
{

public :	
	
	Particle( int x, int y, double dx, double dy );
	~Particle();

	void addForce( double fx, double fy );
	void update();
	void draw();
	
    int x() { return (int)floor(_x); };
    int y() { return (int)floor(_y); };
	
	double _x;
	double _y;
	double _dx;
	double _dy;
	double _dxt;
	double _dyt;
};