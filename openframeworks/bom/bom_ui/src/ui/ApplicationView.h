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

#include "Model.h"
#include "EventData.h"
#include "KeyData.h"

#include "Reference.h"
#include "Timeline.h"
#include "TimelineTab.h"
#include "TimelinePlayBtn.h"
#include "IntroPanel.h"
#include "YearPanel.h"
#include "KeyPanel.h"
#include "EventPanelManager.h"

class ApplicationView : public ofxStage
{

public:
	
	ApplicationView() 
	{
		addListeners();
	};
	
	~ApplicationView() {};
	
	//==================================================
	
	Model*				model;
	EventData			eventData;
	KeyData				keyRainData;
	KeyData				keyTempData;
	
	Reference*			ref;
	TimelineTab*		timelineTab;
	TimelinePlayBtn*	timelinePlayBtn;
	Timeline*			timeline;
	IntroPanel*			introPanel;
	YearPanel*			yearPanel;
	KeyPanel*			keyPanel;
	EventPanelManager*	eventPanels;
	
	//==================================================
	
	void setup ()
	{
		eventData.loadFile( "xml/events.xml" );
		keyRainData.loadFile( "xml/keys_rain.xml" );
		keyTempData.loadFile( "xml/keys_temp.xml" );
		
		model = Model :: getInstance();
		model->setEventData( eventData.items );
		model->setKeyRainData( keyRainData.items );
		model->setKeyTempData( keyTempData.items );
		
//		addChild( ref = new Reference() );
		addChild( timelineTab = new TimelineTab() );
		addChild( timelinePlayBtn = new TimelinePlayBtn() );
		addChild( timeline = new Timeline() );
		addChild( introPanel = new IntroPanel() );
		addChild( yearPanel = new YearPanel() );
		addChild( keyPanel = new KeyPanel() );
		addChild( eventPanels = new EventPanelManager() );
	}
	
};