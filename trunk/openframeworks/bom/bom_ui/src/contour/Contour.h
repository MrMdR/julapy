/*
 *  Contour.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 24/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Model.h"
#include "ContourAnim.h"
#include "ofxOpenCv.h"
#include "ofxContourUtil.h"
#include "ofxResizeUtil.h"
#include "ofxSimpleGuiToo.h"

#include "KeyDataItem.h"

class Blob : public ofxCvBlob
{
public:
	ofColor	color;
	int		layer;
};


class Contour
{

public:
	
	 Contour();
	~Contour();
	
	//==================================================
	
	Model*					model;
	
	bool					bDebug;
	bool					bProgressChanged;
	bool					bTabChanged;
	
	ofRectangle				screenRect;
	ofRectangle				sourceRect;
	ofRectangle				largeRect;
	ofRectangle				smallRect;
	ofRectangle				debugRect;
	
	ContourAnim				bomTempAnim;
	ContourAnim				bomRainAnim;
	ContourAnim*			bomAnim;
	ofxCvGrayscaleImage		bomImage;
	float					bomAnimProgress;
	float					bomAninSpeed;
	
	ofxCvGrayscaleImage		bandSum;
	ofxCvGrayscaleImage*	bands;
	float*					bandCutoffs;
	int						bandsTotal;
	int						bandIndex;
	ofColor*				bandRainColors;
	ofColor*				bandTempColors;
	ofColor*				bandBaseColor;
	
	ofxCvContourFinder		contourFinder;
	ofxContourUtil			contourUtil;
	vector<Blob>			contourBlobs;
	vector<Blob>			contourBlobsScaled;
	float					contourSmoothScale;
	float					contourSimplifyScale;
	float					contourSimplifyTolerance;
	
	//==================================================

	void setup			();
	void update			();
	
	void initSize		();
	void initAnim		();
	void initOpenCv		();
	void initContours	();
	void initGui		();
	
	bool updateAnim				();
	void updateOpenCv			();
	void updateBlobs			();
	int  updateContours			( ofxCvGrayscaleImage& image );
	void copyBlob				( ofxCvBlob& blob, ofxCvBlob& blobCopy, float xoff = 0, float yoff = 0, float scale = 1.0 );
	
	void progressChangeEventHandler ( float& p );
	void tabTypeChangeEventHandler	( int& t );
	
};