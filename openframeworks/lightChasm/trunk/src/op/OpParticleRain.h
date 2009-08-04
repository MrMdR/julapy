/*
 *  OpParticleRain.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 25/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _H_OP_PARTICLE_RAIN
#define _H_OP_PARTICLE_RAIN

#include "ofMain.h"
#include "ofFBOTexture.h"

class OpParticleRainDrop
{
public :
	float loc[ 2 ];
	float vel[ 2 ];
	float size;
	float wobbleMag;
	float wobbleAng;
	float wobbleAngInc;
	bool  alive;
};

class OpParticleRain
{

public :
	
	OpParticleRain();
	~OpParticleRain();
	
	void init( int w, int h );
	void update();
	unsigned char* getPixels();
	
	void drawToFBO();
	void drawFBOToScreen();
	void draw();

	void setMinParticleSize( float value );
	void setMaxParticleSize( float value );
	void setMinParticleVel( float value );
	void setMaxParticleVel( float value );
	void setMakeRate( float value );
	
private :
	
	int width;
	int height;
	ofFBOTexture fbo;

	vector <OpParticleRainDrop> particles;
	float minParticleSize;
	float maxParticleSize;
	float minParticleVel;
	float maxParticleVel;
	float makeRate;
	
};

#endif