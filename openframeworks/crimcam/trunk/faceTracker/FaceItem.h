/*
 *  FaceItem.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 1/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "FaceItemRectangle.h"

class FaceItem
{
	
public :
	
	FaceItem();
	~FaceItem();
	
	void setID( int fid );
	int  getID();
	
	void set( float x, float y, float w, float h, float cx, float cy );
	void add( float x, float y, float w, float h, float cx, float cy );
	
	bool checkFace( float x, float y, float w, float h, float cx, float cy );
	bool hasBeenMatched();
	bool isFaceIdle();
	bool isValid();
	
	void update();
	void easeToLastRect();
	
	int							faceID;
	
	int							rectBufferCount;
	int							rectBufferMin;
	int							rectBufferLimit;
	float						rectEaseScale;
	
	vector<FaceItemRectangle>	rectBuffer;
	FaceItemRectangle			rect;
	FaceItemRectangle			rectEase;
	
	Boolean						isMatch;
	Boolean						isIdle;
	int							idleCount;
	int							idleCountLimit;
	int							centreDistanceLimit;
	
private :
	
	bool checkAgainstCentreDistance( float cx, float cy );
	void clearRectBuffer();

};