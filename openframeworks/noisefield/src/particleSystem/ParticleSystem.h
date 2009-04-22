/*
 *  ParticleSystem.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Particle.h"

class ParticleSystem
{

public :
	
	ParticleSystem( int w, int h );
	~ParticleSystem();

	void		addParticle( int x, int y, double dx, double dy );
	Boolean		hasNextParticle();
	Particle*	getParticle();
	void		checkBounds();
	void		update();
	void		draw();
	
private :
	
	int					width;
	int					height;
	
	vector <Particle>	particles;
	int					particlesTotal;
	int					particleCount;
	int					particleBoundsGap;
};