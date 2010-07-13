/*
 *  Rocks.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 13/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ROCKS
#define ROCKS

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBlobUtil.h"
#include <fstream>

class Rocks
{

public :
	
	 Rocks();
	~Rocks();
	
	void setup					();
	void update					();
	void draw					();
	
	int  findBlobs				();
	
	void drawImage				();
	void drawBlobs				();
	void drawDownsampledBlobs	();
	
	void save					( string fileName = "rockData" );
	void load					( string fileName = "rockData" );
	
	ofImage					rocksImage;
	ofxCvColorImage			rocksColorImage;
	ofxCvGrayscaleImage		rocksGrayImage;
	
	ofxCvContourFinder		contourFinder;
	ofxCvBlobUtil			blobUtil;
	
	vector<ofxCvBlob>		blobs;
	
	int						threshold;
};

#endif