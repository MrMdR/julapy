/*
 *  FaceTracker.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "FaceItem.h"
#include "ofxCvHaarFinder.h"


class FaceTracker
{

public :	

	FaceTracker();
	~FaceTracker();
	
	void init( int w, int h );
	void findFaces( ofxCvGrayscaleImage *image );
	void getFaces();
	
	ofxCvHaarFinder		haarFinder;
	vector<FaceItem>	faces;
	
};