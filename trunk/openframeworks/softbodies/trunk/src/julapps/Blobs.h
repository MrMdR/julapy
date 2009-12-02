/*
 *  Blobs.h
 *  julapy_softbodies
 *
 *  Created by lukasz karluk on 28/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BLOBS_BLOB_H_
#define _BLOBS_BLOB_H_

class Blob
	{
		public :
		
		vector <ofxBox2dCircle>		nodes;
		vector <ofxBox2dJoint>		nodeJoints;
		int							nodesTotal;
		float						nodeRadius;
		float						*nodeScale;
	};

#endif


#ifndef _BLOBS_BALL_H_
#define _BLOBS_BALL_H_

class Ball
	{
		public :
		
		ofxBox2dCircle		circle;
		float				circleRadius;
	};

#endif
