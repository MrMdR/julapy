/*
 *  ofxContourUtil.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OFX_CV_BLOB_UTIL
#define OFX_CV_BLOB_UTIL

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofxCvBlobUtil
{

public :
	
	 ofxCvBlobUtil();
	ofxCvBlobUtil();
	
	void downSampleBlobs	( vector<ofxCvBlob> &blobsIn, vector<ofxCvBlob> &blobsOut, int numOfPoints );
	void scaleBlobs			( vector<ofxCvBlob> &blobsIn, float scaleX, float scaleY );
	void scaleBlobsToCenter	( vector<ofxCvBlob> &blobsIn, ofPoint &center, float radiusX, float radiusY );
	void translateBlobs		( vector<ofxCvBlob> &blobsIn, float x, float y );
};

#endif