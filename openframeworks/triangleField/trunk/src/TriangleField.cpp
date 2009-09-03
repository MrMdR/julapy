/*
 *  TriangleField.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleField.h"

TriangleField :: TriangleField()
{

}

TriangleField :: ~TriangleField()
{

}

void TriangleField :: copyTo( TriangleField *tf )
{
	tf->scale		= scale;
	tf->scaleInc	= scaleInc;
	tf->cutoff		= cutoff;
	
	copyColor( &tf->sColor[ 0 ], &sColor[ 0 ] );
	copyColor( &tf->eColor[ 0 ], &eColor[ 0 ] );
	copyColor( &tf->bsColor[ 0 ], &bsColor[ 0 ] );
	copyColor( &tf->beColor[ 0 ], &beColor[ 0 ] );
	
	tf->drawOutline	= drawOutline;
	
	tf->noiseX		= noiseX;
	tf->noiseY		= noiseY;
	tf->noiseZ		= noiseZ;
	
	tf->noiseXInit	= noiseXInit;
	tf->noiseYInit	= noiseYInit;
	tf->noiseZInit	= noiseZInit;
	
	tf->noiseXRes	= noiseXRes;
	tf->noiseYRes	= noiseYRes;
	tf->noiseZRes	= noiseZRes;
	
	tf->noiseXVel	= noiseXVel;
	tf->noiseYVel	= noiseYVel;
	tf->noiseZVel	= noiseZVel;
	
	tf->noiseXScl	= noiseXScl;
	tf->noiseYScl	= noiseYScl;
}

void TriangleField :: copyColor( float *c1, float *c2 )
{
	c1[ 0 ] = c2[ 0 ];
	c1[ 1 ] = c2[ 1 ];
	c1[ 2 ] = c2[ 2 ];
	c1[ 3 ] = c2[ 3 ];
}