/*
 *  NawlzParticle01.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "NawlzParticle.h"

class NawlzParticle01 : public NawlzParticle
{
	
public:
	
	NawlzParticle01( ofTexture* texIn, const Vec2f& pos, const Vec2f& vel ) : NawlzParticle( texIn, pos, vel )
	{
		//
	}
	
	void draw ()
	{
		ofFill();
		ofSetColor( 255, 0, 0, 255 * alpha );
		ofRect( pos.x - width * 0.5, pos.y - height * 0.5, width, height );
	}
};
