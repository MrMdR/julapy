/*
 *  EventPanelManager.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 23/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxFlash.h"

#include "Model.h"

#include "EventPanel.h"
#include "EventItem.h"
#include "EventImageItem.h"
#include "EventVideoItem.h"
#include "EventTextItem.h"
#include "EventCover.h"

class EventManager : public ofxSprite
{

public:
	
	EventManager()
	{
		setup();
	}
	
	~EventManager(){}
	
	//==================================================
	
	Model*			model;
	int				eventID;
	int				selectedEventID;
	
	EventPanel*		eventPanel;
	EventItem*		eventItem;
	EventCover*		eventCover;
	
	//==================================================
	
	void setup ()
	{
		eventPanel		= NULL;
		eventItem		= NULL;
		eventCover		= NULL;
		
		eventID			= -1;
		selectedEventID	= -1;
		
		model = Model :: getInstance();
		
		ofAddListener( model->timelineMarkerPressedEvent, this, &EventManager :: timelineMarkerPressedEventHandler );
	}
	
	void timelineMarkerPressedEventHandler ( int & eventID )
	{
		if( this->eventID == eventID )
			return;
		
		this->eventID = eventID;
		
		if( eventID >= 0 )
		{
			initEventPanel();
		}
		else
		{
			killEventPanel();
		}
	}
	
	void initEventPanel ()
	{
		killEventPanel();
		
		eventPanel = new EventPanel();
		eventPanel->populate( model->getEventDataItemAt( eventID ) );
		eventPanel->setup();
		
		ofAddListener( eventPanel->eventPanelPressedEvent, this, &EventManager :: eventPanelPressedHandler );
		
		addChild( eventPanel );
	}
	
	void killEventPanel ()
	{
		if( eventPanel == NULL )
			return;
		
		removeChild( eventPanel );
		
		ofRemoveListener( eventPanel->eventPanelPressedEvent, this, &EventManager :: eventPanelPressedHandler );
		
		delete eventPanel;
		eventPanel = NULL;
	}
	
	void eventPanelPressedHandler ( int& eventID )
	{
		selectedEventID = eventID;
		
		initCover();
		initEvent();
	}
	
	//==================================================
	
	void initCover ()
	{
		killCover();
		
		eventCover = new EventCover();
		addChild( eventCover );
	}
	
	void killCover ()
	{
		if( eventCover == NULL )
			return;
		
		removeChild( eventCover );
		
		delete eventCover;
		eventCover = NULL;
	}
	
	void initEvent ()
	{
		killEvent();
		
		const EventDataItem& data = model->getEventDataItemAt( selectedEventID );
		
		if( data.type == EVENT_TYPE_IMAGE )
		{
			eventItem = new EventImageItem();
		}
		else if( data.type == EVENT_TYPE_VIDEO )
		{
			eventItem = new EventVideoItem();
		}
		else if( data.type == EVENT_TYPE_TEXT )
		{
			eventItem = new EventTextItem();
		}
		
		eventItem->populate( data );
		eventItem->setup();
		eventItem->show();
		
		ofAddListener( eventItem->closeEvent, this, &EventManager :: eventItemCloseHandler );
		ofAddListener( ofEvents.mousePressed, this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void killEvent ()
	{
		if( eventItem == NULL )
			return;
		
		removeChild( eventItem );
		
		ofRemoveListener( eventItem->closeEvent, this, &EventManager :: eventItemCloseHandler );
		ofRemoveListener( ofEvents.mousePressed, this, &EventManager :: mousePressedHandler );
		
		delete eventItem;
		eventItem = NULL;
	}
	
	void eventItemCloseHandler ( int& eventID )
	{
		killCover();
		killEvent();
	}
	
	void mousePressedHandler ( ofMouseEventArgs &e )
	{
		killCover();
		killEvent();
	}
	
	//==================================================
	
	void draw ()
	{
		//
	}
};