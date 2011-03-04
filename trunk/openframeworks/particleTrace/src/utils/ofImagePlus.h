/*
 *  ofImagePlus.h
 *  PopAdidas
 *
 *  Created by kikko on 9/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class ofImagePlus : public ofImage {
	
public:
	void mirror(bool horizontal, bool vertical);
	void setVertical();
	
protected:
	void flipPixels(ofPixels &pix, bool horizontal, bool vertical);
};