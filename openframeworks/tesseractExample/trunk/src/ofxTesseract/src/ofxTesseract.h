/*
 *  ofxTesseract.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 21/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_TESSERACT_
#define _OFX_TESSERACT_

#include "ofMain.h"
#include "baseapi.h"

class ofxTesseract
{

public :
	
	ofxTesseract();
	
	void	setROI			( const ofRectangle &roi );
	void	resetROI		( );
	string	findTextInImage	( ofImage &image );
	string	findTextInImage	( const unsigned char* pixels, int width, int height, int type );
	
protected :
	
	TessBaseAPI*	tess;
	
	ofRectangle		roi;
	bool			bUseRoi;
	
};

#endif _OFX_TESSERACT_