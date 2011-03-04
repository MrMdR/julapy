/*
 *  ofImagePlus.cpp
 *  PopAdidas
 *
 *  Created by kikko on 9/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofImagePlus.h"

void ofImagePlus::mirror(bool horizontal, bool vertical){
	flipPixels(myPixels, horizontal, vertical);
	
	update();
}

//----------------------------------------------------
void ofImagePlus::flipPixels(ofPixels &pix, bool horizontal, bool vertical){
	if(!horizontal && !vertical)
		return;
	
	FIBITMAP * bmp = getBmpFromPixels(pix);
	bool horSuccess = false, vertSuccess = false;
	
	if(horizontal)
		horSuccess = FreeImage_FlipHorizontal(bmp);
	if(vertical)
		vertSuccess = FreeImage_FlipVertical(bmp);
	
	if(horSuccess || vertSuccess)
		putBmpIntoPixels(bmp, pix);
	
	if (bmp != NULL)            FreeImage_Unload(bmp);
}

void ofImagePlus::setVertical() {
	
	int w = (int) getWidth();
	int h = (int) getHeight();
	
	ofImage temp;
	temp.allocate(h, w, type);
	
	unsigned char* srcPixels = getPixels();
	unsigned char* destPixels = temp.getPixels();
	
	int destPosition = 0, srcPosition = 0;
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			destPosition = (x * h + y) * 3;
			srcPosition = (y * w + (w-x-1)) * 3;
			destPixels[destPosition + 0] = srcPixels[srcPosition + 0];
			destPixels[destPosition + 1] = srcPixels[srcPosition + 1];
			destPixels[destPosition + 2] = srcPixels[srcPosition + 2];
		}
	}
	
	allocate(h, w, type);
	setFromPixels(destPixels, h, w, type);
}