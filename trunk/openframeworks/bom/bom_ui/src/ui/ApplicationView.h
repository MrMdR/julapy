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

#include "Reference.h"
#include "Timeline.h"
#include "TimelineTab.h"
#include "TimelinePlayBtn.h"
#include "IntroPanel.h"
#include "YearPanel.h"

#include "EventPanel.h"

class ApplicationView : public ofxStage
{

public:
	
	ApplicationView() 
	{
		addListeners();
	};
	
	~ApplicationView() {};
	
	//==================================================
	
	Reference			ref;
	TimelineTab			timelineTab;
	TimelinePlayBtn		timelinePlayBtn;
	Timeline			timeline;
	IntroPanel			introPanel;
	YearPanel			yearPanel;
	
	EventPanel			eventPanel;
	
	//==================================================
	
	void setup ()
	{
//		addChild( &ref );
		addChild( &timelineTab );
		addChild( &timelinePlayBtn );
		addChild( &timeline );
		addChild( &introPanel );
		addChild( &yearPanel );
		addChild( &eventPanel );
		
		eventPanel.setThumbImage( (ofImage*)ofxAssets :: getInstance()->getAsset( "event_thumb_0" ) );
	}
	
};