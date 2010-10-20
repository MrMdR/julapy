/*
 *  ApplicationView.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 20/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"

#include "TimelineTab.h"

class ApplicationView : public ofxStage
{

public:
	
	ApplicationView() 
	{
		addListeners();
	};
	
	~ApplicationView() {};
	
	//==================================================
	
	TimelineTab		timelineTab;
	
	//==================================================
	
	void setup ()
	{
		addChild( &timelineTab );
	}
	
};