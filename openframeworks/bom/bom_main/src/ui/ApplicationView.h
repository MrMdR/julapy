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
#include "InteractionTracker.h"
#include "SoundManager.h"

#include "EventData.h"
#include "KeyData.h"

#include "Contour.h"
#include "ContourMap.h"

#include "Reference.h"
#include "Timeline.h"
#include "TimelineTab.h"
#include "TimelinePlayBtn.h"
#include "IntroPanel.h"
#include "YearPanel.h"
#include "KeyPanel.h"
#include "ElNinoBtn.h"
#include "EventManager.h"

class ApplicationView : public ofxStage
{

public:
	
	ApplicationView() 
	{
		contour = NULL;
		
		addListeners();
	};
	
	~ApplicationView() {};
	
	//==================================================

	SoundManager*		sound;
	InteractionTracker*	tracker;
	
	Model*				model;
	EventData			eventData;
	KeyData				keyRainData;
	KeyData				keyTempData;
	
	Contour*			contour;
	ContourMap*			contourMap;
	
	Reference*			ref;
	TimelineTab*		timelineTab;
	TimelinePlayBtn*	timelinePlayBtn;
	Timeline*			timeline;
	IntroPanel*			introPanel;
	YearPanel*			yearPanel;
	KeyPanel*			keyPanel;
	ElNinoBtn*			elninoBtn;
	EventManager*		eventManager;
	
	//==================================================
	
	void setup ()
	{
		tracker = InteractionTracker :: getInstance();
		sound	= SoundManager :: getInstance();
		
		eventData.loadFile( "xml/events.xml" );
		keyRainData.loadFile( "xml/keys_rain.xml" );
		keyTempData.loadFile( "xml/keys_temp.xml" );
		
		model = Model :: getInstance();
		model->setEventData( eventData.items );
		model->setKeyRainData( keyRainData.items );
		model->setKeyTempData( keyTempData.items );

		ofRectangle pixelRect;
		pixelRect.x			= 272;		// dimensions from alex's design.
		pixelRect.y			= -5;
		pixelRect.width		= 757;
		pixelRect.height	= 590;

		ofRectangle earthRect;
		earthRect.x			= 112;		// dimensions from bom data.
		earthRect.y			= 9.95;		// this is really a minus value, but im making it positive for simplicity.
		earthRect.width		= 44.3;
		earthRect.height	= 34.55;
		
		model->setPixelRect( pixelRect );
		model->setEarthRect( earthRect );
		
		contour = new Contour();
		
		addChild( contourMap = new ContourMap( contour ) );
		
//		addChild( ref = new Reference() );						// reference image, only temp.
		addChild( timelineTab = new TimelineTab() );
		addChild( timelinePlayBtn = new TimelinePlayBtn() );
		addChild( timeline = new Timeline() );
		addChild( introPanel = new IntroPanel() );
		addChild( yearPanel = new YearPanel() );
		addChild( keyPanel = new KeyPanel() );
		addChild( elninoBtn = new ElNinoBtn() );
		addChild( eventManager = new EventManager() );
	}
	
	void update ()
	{
		ofxStage :: update();
	
		if( contour != NULL )
			contour->update();
	}
	
	void draw ()
	{
		ofxStage :: draw();
		
		ofSetColor( 0x000000 );
//		ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ) + " fps" , ofGetWidth() - 60, 15 );
	}
};