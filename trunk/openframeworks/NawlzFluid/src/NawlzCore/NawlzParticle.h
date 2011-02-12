/*
 *  NawlzParticle.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "MSACore.h"
#include "ofBaseApp.h"
#include "ofTexture.h"

using namespace MSA;

class NawlzParticle : public ofBaseApp
{
	
public:
	
	 NawlzParticle( ofTexture* texIn, const Vec2f& pos, const Vec2f& vel );
	~NawlzParticle();
	
	virtual void draw ();
	
	ofTexture*	tex;
	int			width;
	int			height;
	float		alpha;
	float		mass;
	Vec2f		pos;
	Vec2f		vel;
	
};
