/*
 *  ofxTrigLUT.cpp
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxTrigLUT.h"

float  ofxTrigLUT :: precision			= 0;
int    ofxTrigLUT :: length				= 0;
float* ofxTrigLUT :: sinLUT				= new float[ 1 ];		// static variables need to be defined inside the cpp file.
float* ofxTrigLUT :: cosLUT				= new float[ 1 ];		// don't know any other way of defining this but to create a dummy array.

void ofxTrigLUT :: createLUT ( float p )
{
	precision	= p;
	length		= (int)( 360.0 / precision );
	
	delete [] sinLUT;
	delete [] cosLUT;
	
	sinLUT = new float[ length ];
	cosLUT = new float[ length ];
	
	for( int i=0; i<length; i++ )
	{
		sinLUT[ i ] = sin( i * DEG_TO_RAD * precision );
		cosLUT[ i ] = cos( i * DEG_TO_RAD * precision );
	}
}