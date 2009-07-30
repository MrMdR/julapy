/*
 *  CurveHop.h
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 10/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Ribbon.h"

class CurveHop : public Ribbon
{
public :
	
	CurveHop();
	~CurveHop();
	
private :
	
	void calcLine();
};