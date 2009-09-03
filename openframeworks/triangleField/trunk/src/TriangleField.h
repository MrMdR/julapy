/*
 *  TriangleField.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TRIANGLE_FIELD_H_
#define _TRIANGLE_FIELD_H_

class TriangleField 
{

public :
	
	TriangleField();
	~TriangleField();
	
	void copyTo( TriangleField *tf );
	void copyColor( float *c1, float *c2 );
	
	float		sColor[ 4 ];
	float		eColor[ 4 ];
	float		bsColor[ 4 ];
	float		beColor[ 4 ];
	
	float		scale;
	int			scaleInc;
	float		cutoff;
	
	bool		drawOutline;
	
	float		noiseX;
	float		noiseY;
	float		noiseZ;
	
	float		noiseXInit;
	float		noiseYInit;
	float		noiseZInit;
	
	float		noiseXRes;
	float		noiseYRes;
	float		noiseZRes;
	
	float		noiseXVel;
	float		noiseYVel;
	float		noiseZVel;
	
	float		noiseXScl;
	float		noiseYScl;
	
};

#endif