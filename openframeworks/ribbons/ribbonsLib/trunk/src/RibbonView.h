/*
 *  RibbonView.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 27/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RIBBON_VIEW_H_
#define _RIBBON_VIEW_H_

#define		RIBBON_VIEW_FILL_TYPE		1
#define		RIBBON_VIEW_MESH_TYPE		2

#include "Ribbon.h"

class RibbonView
{
	
public :
	
	RibbonView();
	~RibbonView();
	
	virtual void setup( Ribbon *r );
	virtual void draw();
	
	void setType( int value );
	int  getType();
	
	Ribbon *ribbon;
	int type;

};

#endif