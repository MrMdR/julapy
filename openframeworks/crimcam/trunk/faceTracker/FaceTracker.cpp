/*
 *  FaceTracker.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "FaceTracker.h"

FaceTracker :: FaceTracker()
{
	//
}

FaceTracker :: ~FaceTracker()
{
	
}

void FaceTracker :: init( int w, int h )
{
	haarFinder.setup( "haarXML/haarcascade_frontalface_default.xml" );
}

void FaceTracker :: findFaces( ofxCvGrayscaleImage *image )
{
	haarFinder.findHaarObjects( *image, 10, 99999999, 10 );
}

void FaceTracker :: getFaces()
{
	
}
