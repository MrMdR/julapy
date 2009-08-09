/*
 *  SphereRibbon.h
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 21/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _H_SPHERE_RIBBON
#define _H_SPHERE_RIBBON

#include "Ribbon.h"

class SphereRibbon : public Ribbon
{
public :
	
	SphereRibbon();
	~SphereRibbon();
	
private :
	
	void calcLine();
	
};

#endif