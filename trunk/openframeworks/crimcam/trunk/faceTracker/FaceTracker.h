/*
 *  FaceTracker.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofxCvMain.h"
#include "ofxCvHaarFinder.h"
#include "FaceItem.h"

class FaceTracker
{

public :	

	FaceTracker();
	~FaceTracker();
	
	void		init( int w, int h );
	void		findFaces( ofxCvGrayscaleImage *image );
	bool		hasNextFace();
	int			getFaceID();
	void		getFaceProperties( float *x, float *y, float *w, float *h, float *cx, float *cy );
	
private :	
	
	void clearFaces();
	
	ofxCvHaarFinder		haarFinder;
	vector<FaceItem>	faces;
	int					faceIteratorIndex;
	
	int					faceIDCounter;
	int					faceIDCounterLimit;
	
};