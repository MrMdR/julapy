/*
 *  ofxColorUtils.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 23/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxColorUtils.h"

int ofRgbToHex ( int r, int g, int b )
{
	unsigned int hex;
	
	hex = 
	( ( r & 0xff ) << 16 ) |
	( ( g & 0xff ) << 8  ) |
	( ( b & 0xff ) );
	
	return hex;
}
