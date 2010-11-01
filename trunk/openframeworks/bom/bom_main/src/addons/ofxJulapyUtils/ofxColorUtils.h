/*
 *  ofxColorUtils.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 23/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_COLOR_UTILS_H_
#define _OFX_COLOR_UTILS_H_

struct RgbColor
{
	int r;
	int g;
	int b;
};


int			ofRgbToHex ( int r, int g, int b );
RgbColor	ofHexToRgb ( int hex );

#endif