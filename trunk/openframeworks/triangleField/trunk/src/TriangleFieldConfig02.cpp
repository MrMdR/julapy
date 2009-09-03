/*
 *  TriangleFieldConfig02.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleFieldConfig02.h"

TriangleFieldConfig02 :: TriangleFieldConfig02()
{
	init();
}

TriangleFieldConfig02 :: ~TriangleFieldConfig02()
{
	
}

void TriangleFieldConfig02 :: init()
{
	TriangleFieldConfig :: init();
	
	fieldsTotal = 3;
	fields = new TriangleField[ fieldsTotal ];
	
	int sr = 255;		// start RGBA
	int sg = 0;
	int sb = 0;
	int sa = 255;
	
	int er = 255;		// end RGBA
	int eg = 0;
	int eb = 0;
	int ea = 255;
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		fields[ i ].sColor[ 0 ] = sr;
		fields[ i ].sColor[ 1 ] = sg;
		fields[ i ].sColor[ 2 ] = sb;
		fields[ i ].sColor[ 3 ] = sa;
		
		fields[ i ].eColor[ 0 ] = er;
		fields[ i ].eColor[ 1 ] = eg;
		fields[ i ].eColor[ 2 ] = eb;
		fields[ i ].eColor[ 3 ] = ea;
		
		fields[ i ].bsColor[ 0 ] = sr;
		fields[ i ].bsColor[ 1 ] = sg;
		fields[ i ].bsColor[ 2 ] = sb;
		fields[ i ].bsColor[ 3 ] = sa;
		
		fields[ i ].beColor[ 0 ] = er;
		fields[ i ].beColor[ 1 ] = eg;
		fields[ i ].beColor[ 2 ] = eb;
		fields[ i ].beColor[ 3 ] = ea;
		
		fields[ i ].scale    = 1;
		fields[ i ].scaleInc = 1;
		fields[ i ].cutoff   = 1.0;
		
		fields[ i ].drawOutline = false;
		
		fields[ i ].noiseX = 0.0;
		fields[ i ].noiseY = 0.0;
		fields[ i ].noiseZ = 0.0;
		
		fields[ i ].noiseXInit = 0.0;
		fields[ i ].noiseYInit = 0.0;
		fields[ i ].noiseZInit = 0.0;
		
		fields[ i ].noiseXRes = 0.01;
		fields[ i ].noiseYRes = 0.01;
		fields[ i ].noiseZRes = 0.01;
		
		fields[ i ].noiseXVel = 0.0;
		fields[ i ].noiseYVel = 0.0;
		fields[ i ].noiseZVel = 0.005;
		
		fields[ i ].noiseXScl = 1.0;
		fields[ i ].noiseYScl = 1.0;
	}
	
	fields[ 0 ].scaleInc  = 3;
	fields[ 0 ].scale     = pow( 2, fields[ 0 ].scaleInc );
	fields[ 0 ].cutoff    = 0.0;
	fields[ 0 ].noiseXVel = 0.0;
	fields[ 0 ].noiseYVel = 0.01;
	fields[ 0 ].drawOutline = true;
	
	fields[ 1 ].scaleInc  = 3;
	fields[ 1 ].scale     = pow( 2, fields[ 1 ].scaleInc );
	fields[ 1 ].cutoff    = 0.6;
	fields[ 1 ].noiseXVel = 0.0;
	fields[ 1 ].noiseYVel = -0.01;
	
	fields[ 2 ].scaleInc  = 2;
	fields[ 2 ].scale     = pow( 2, fields[ 2 ].scaleInc );
	fields[ 2 ].cutoff    = 0.4;
	fields[ 2 ].noiseXVel = 0.0;
	fields[ 2 ].noiseYVel = 0.02;
}

void TriangleFieldConfig02 :: update()
{
	TriangleFieldConfig :: update();
	
	float c1, c2;
	int i;
	
	i = 0;
	
	//	fields[ i ].scaleInc = (int)( 1 + 4 * audio.getAveragePeakNorm() );
	//	fields[ i ].scale = pow( 2, fields[ i ].scaleInc );
	
	i = 1;
	
	c1 = 0.3 + 0.5 * ( 1 - audioNorm );
	c2 = fields[ i ].cutoff;
	
	fields[ i ].cutoff += ( c1 - c2 ) * 0.5;
	
	i = 2;
	
	c1 = 0.3 + 0.5 * ( 1 - audioNorm );
	c2 = fields[ i ].cutoff;
	
	fields[ i ].cutoff += ( c1 - c2 ) * 0.5;
}

void TriangleFieldConfig02 :: setSize( float w, float h )
{
	TriangleFieldConfig :: setSize( w, h );
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		fields[ i ].noiseXScl = height / (float)width;
		fields[ i ].noiseYScl = 1.0;
	}
}

void TriangleFieldConfig02 :: copyChangesTo( TriangleField *tfs, int tfTotal )
{
	int i = 1;
	
	tfs[ i ].cutoff = fields[ i ].cutoff;
	
	i = 2;
	
	tfs[ i ].cutoff = fields[ i ].cutoff;
}
