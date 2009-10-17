//
// ofxCvOpticalFlowLK.h - a OpenCV cvOpticalFlowLK warpper for openFrameworks
//
// Copyright (C) 2008 Takashi Maekawa <takachin@generative.info> 
//     All rights reserved.
//     This is free software with ABSOLUTELY NO WARRANTY.
//
// You can redistribute it and/or modify it under the terms of 
// the GNU Lesser General Public License.
//

#pragma once

#include "ofMain.h"
#include "ofxCvConstants.h"
#include "ofxCvGrayscaleImage.h"

class ofCvOpticalFlowLK
{
public:

	ofCvOpticalFlowLK(void);
	~ofCvOpticalFlowLK(void);
		
	void allocate(int _w, int _h);

	void calc( ofxCvGrayscaleImage & pastImage, ofxCvGrayscaleImage & currentImage, int size);

	void reset();

	int captureWidth;
	int captureHeight;
	
	IplImage* vel_x;
	IplImage* vel_y;

private:

	static const int DEFAULT_CAPTURE_WIDTH = 320;
	static const int DEFAULT_CAPTURE_HEIGHT = 240;

};
