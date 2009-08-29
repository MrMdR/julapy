/*
 *  RibbonView.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 27/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonView.h"

RibbonView :: RibbonView()
{
	//
}

RibbonView :: ~RibbonView()
{
	//
}

void RibbonView :: setup( Ribbon *r )
{
	ribbon = r;
}

void RibbonView :: draw()
{

}

void RibbonView :: setType( int value )
{
	type = value;
}

int RibbonView :: getType()
{
	return type;
}