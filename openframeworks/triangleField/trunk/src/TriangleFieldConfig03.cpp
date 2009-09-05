/*
 *  TriangleFieldConfig03.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 3/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "TriangleFieldConfig03.h"

TriangleFieldConfig03 :: TriangleFieldConfig03()
{
	init();
}

TriangleFieldConfig03 :: ~TriangleFieldConfig03()
{
	
}

void TriangleFieldConfig03 :: init()
{
	TriangleFieldConfig :: init();
	
	fieldsTotal = 3;
	fields = new TriangleField[ fieldsTotal ];
	
	int sr = 108;		// start RGBA
	int sg = 83;
	int sb = 86;
	int sa = 255;
	
	int er = 168;		// end RGBA
	int eg = 196;
	int eb = 170;
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
		fields[ i ].noiseZVel = 0.03;
		
		fields[ i ].noiseXScl = 1.0;
		fields[ i ].noiseYScl = 1.0;
	}
	
	fields[ 0 ].scaleInc  = 3;
	fields[ 0 ].scale     = pow( 2, fields[ 0 ].scaleInc );
	fields[ 0 ].cutoff    = 0.0;
	fields[ 0 ].noiseXVel = 0.0;
	fields[ 0 ].noiseYVel = 0.01;
	fields[ 0 ].drawOutline = true;
	
	sr = 0;			// start RGBA
	sg = 0;
	sb = 0;
	sa = 255;
	
	er = 255;		// end RGBA
	eg = 221;
	eb = 191;
	ea = 255;
	
	fields[ 0 ].sColor[ 0 ] = sr;
	fields[ 0 ].sColor[ 1 ] = sg;
	fields[ 0 ].sColor[ 2 ] = sb;
	fields[ 0 ].sColor[ 3 ] = sa;
	fields[ 0 ].eColor[ 0 ] = er;
	fields[ 0 ].eColor[ 1 ] = eg;
	fields[ 0 ].eColor[ 2 ] = eb;
	fields[ 0 ].eColor[ 3 ] = ea;
	fields[ 0 ].bsColor[ 0 ] = sr;
	fields[ 0 ].bsColor[ 1 ] = sg;
	fields[ 0 ].bsColor[ 2 ] = sb;
	fields[ 0 ].bsColor[ 3 ] = sa;
	fields[ 0 ].beColor[ 0 ] = er;
	fields[ 0 ].beColor[ 1 ] = eg;
	fields[ 0 ].beColor[ 2 ] = eb;
	fields[ 0 ].beColor[ 3 ] = ea;
	
	fields[ 1 ].scaleInc  = 2;
	fields[ 1 ].scale     = pow( 2, fields[ 1 ].scaleInc );
	fields[ 1 ].cutoff    = 0.5;
	fields[ 1 ].noiseXRes = 0.00083;
	fields[ 1 ].noiseYRes = 0.041;
	fields[ 1 ].noiseZRes = 0.0;
	fields[ 1 ].noiseXVel = 0.01;
	fields[ 1 ].noiseYVel = -0.02;
	fields[ 1 ].noiseZVel = 0.00083;
	
	sr = 96;		// start RGBA
	sg = 75;
	sb = 70;
	sa = 255;
	
	er = 255;		// end RGBA
	eg = 111;
	eb = 243;
	ea = 255;
	
	fields[ 1 ].sColor[ 0 ] = sr;
	fields[ 1 ].sColor[ 1 ] = sg;
	fields[ 1 ].sColor[ 2 ] = sb;
	fields[ 1 ].sColor[ 3 ] = sa;
	fields[ 1 ].eColor[ 0 ] = er;
	fields[ 1 ].eColor[ 1 ] = eg;
	fields[ 1 ].eColor[ 2 ] = eb;
	fields[ 1 ].eColor[ 3 ] = ea;
	fields[ 1 ].bsColor[ 0 ] = sr;
	fields[ 1 ].bsColor[ 1 ] = sg;
	fields[ 1 ].bsColor[ 2 ] = sb;
	fields[ 1 ].bsColor[ 3 ] = sa;
	fields[ 1 ].beColor[ 0 ] = er;
	fields[ 1 ].beColor[ 1 ] = eg;
	fields[ 1 ].beColor[ 2 ] = eb;
	fields[ 1 ].beColor[ 3 ] = ea;
	
	fields[ 2 ].scaleInc  = 1;
	fields[ 2 ].scale     = pow( 2, fields[ 2 ].scaleInc );
	fields[ 2 ].cutoff    = 0.5;
	fields[ 2 ].noiseXRes = 0.00083;
	fields[ 2 ].noiseYRes = 0.041;
	fields[ 2 ].noiseZRes = 0.0;
	fields[ 2 ].noiseXVel = -0.01;
	fields[ 2 ].noiseYVel = 0.04;
	fields[ 2 ].noiseZVel = 0.00083;
	
	sr = 93;	// start RGBA
	sg = 102;
	sb = 87;
	sa = 255;
	
	er = 0;		// end RGBA
	eg = 255;
	eb = 26;
	ea = 255;
	
	fields[ 2 ].sColor[ 0 ] = sr;
	fields[ 2 ].sColor[ 1 ] = sg;
	fields[ 2 ].sColor[ 2 ] = sb;
	fields[ 2 ].sColor[ 3 ] = sa;
	fields[ 2 ].eColor[ 0 ] = er;
	fields[ 2 ].eColor[ 1 ] = eg;
	fields[ 2 ].eColor[ 2 ] = eb;
	fields[ 2 ].eColor[ 3 ] = ea;
	fields[ 2 ].bsColor[ 0 ] = sr;
	fields[ 2 ].bsColor[ 1 ] = sg;
	fields[ 2 ].bsColor[ 2 ] = sb;
	fields[ 2 ].bsColor[ 3 ] = sa;
	fields[ 2 ].beColor[ 0 ] = er;
	fields[ 2 ].beColor[ 1 ] = eg;
	fields[ 2 ].beColor[ 2 ] = eb;
	fields[ 2 ].beColor[ 3 ] = ea;
}

void TriangleFieldConfig03 :: update()
{
	TriangleFieldConfig :: update();
	
	float c1, c2;
	int i;
	
	i = 1;
	
	c1 = 0.3 + 0.4 * ( 1 - audioNorm );
	c2 = fields[ i ].cutoff;
	
	fields[ i ].cutoff		+= ( c1 - c2 ) * 0.3;
	fields[ i ].noiseXVel	+= ( ( 0.05 * audioNorm ) - fields[ i ].noiseXVel ) * 0.3;
	
	i = 2;
	
	c1 = 0.3 + 0.4 * ( 1 - audioNorm );
	c2 = fields[ i ].cutoff;
	
	fields[ i ].cutoff		+= ( c1 - c2 ) * 0.3;
	fields[ i ].noiseXVel	+= ( ( -0.05 * audioNorm ) - fields[ i ].noiseXVel ) * 0.3;
	fields[ i ].scaleInc	= (int)( 1 + audioNorm * 2 );
	fields[ i ].scale		= pow( 2, fields[ i ].scaleInc );
}

void TriangleFieldConfig03 :: setSize( float w, float h )
{
	TriangleFieldConfig :: setSize( w, h );
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		fields[ i ].noiseXScl = height / (float)width;
		fields[ i ].noiseYScl = 1.0;
	}
}

void TriangleFieldConfig03 :: copyChangesTo( TriangleField *tfs, int tfTotal )
{
	int i = 1;
	
	tfs[ i ].cutoff		= fields[ i ].cutoff;
	tfs[ i ].noiseXVel	= fields[ i ].noiseXVel;
	
	i = 2;
	
	tfs[ i ].cutoff		= fields[ i ].cutoff;
	tfs[ i ].noiseXVel	= fields[ i ].noiseXVel;
	tfs[ i ].scaleInc	= fields[ i ].scaleInc;
	tfs[ i ].scale		= fields[ i ].scale;
}