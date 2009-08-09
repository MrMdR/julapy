/*
 *  SphereBg.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 8/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SPHERE_BG_H_
#define _SPHERE_BG_H_

#include "ofMain.h"

class SphereBg
{

public :
	
	SphereBg();
	~SphereBg();

	void init();
	void update();
	void draw();
	
	ofImage			sphereImage;
	
	GLUquadricObj	*quadratic;
	float			sphereSize;
	float			sphereOrientation[ 16 ];
	
};

#endif