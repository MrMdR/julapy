/*
 *  ofxResizeUtil.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 19/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_RESIZE_UTIL_H_
#define _OFX_RESIZE_UTIL_H_

#include "ofMain.h"

class ofxResizeUtil
{

public :
	
	static ofRectangle cropToSize( const ofRectangle& srcRect, const ofRectangle& dstRect );
	static ofRectangle fitToSize( const ofRectangle& srcRect, const ofRectangle& dstRect );

};

#endif