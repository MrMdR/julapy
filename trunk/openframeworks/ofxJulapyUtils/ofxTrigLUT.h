/*
 *  ofxTrigLUT.h
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 11/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_TRIG_LUT_H_
#define _OFX_TRIG_LUT_H_

#include "ofMain.h"

class ofxTrigLUT
{
	
public : 
	
	static float *sinLUT;
	static float *cosLUT;
	static float precision;
	static int	 length;
	
	static void  createLUT ( float precision = 1.0 );

};

#endif