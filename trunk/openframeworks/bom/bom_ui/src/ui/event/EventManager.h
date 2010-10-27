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
#include "EventElninoItem.h"
#include "EventFutureItem.h"
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
	
	Model*					model;
	vector<EventDataItem>*	data;
	int						eventID;
	int						selectedEventID;
	
	EventPanel**	eventPanels;
	int				eventPanelsTotal;
	
	EventItem*		eventItem;
	EventCover*		eventCover;
	
	//==================================================
	
	void setup ()
	{
		eventItem		= NULL;
		eventCover		= NULL;
		
		eventID			= -1;
		selectedEventID	= -1;
		
		model	= Model :: getInstance();
		data	= model->getEventData();

		initEventPanels();
		
		ofAddListener( model->eventProgressChangeEvent	,	this, &EventManager :: eventProgressChangeEventHandler );
		ofAddListener( model->futureBtnPressedEvent,		this, &EventManager :: futureBtnPressedEventHandler );
		ofAddListener( model->elninoBtnPressedEvent,		this, &EventManager :: elninoBtnPressedEventHandler );
	}
	
	//==================================================
	
	void eventProgressChangeEventHandler ( float& eventPosition )
	{
		checkEventPanels( eventPosition );
	}
	
	//==================================================
	
	void futureBtnPressedEventHandler ( int& eventID )
	{
		initCover();
		initFutureEvent();
	}
	
	//==================================================
	
	void elninoBtnPressedEventHandler ( int& btnID )
	{
		initCover();
		initElninoEvent();
	}
	
	//==================================================
	
	void initEventPanels ()
	{
		eventPanelsTotal	= data->size();
		eventPanels			= new EventPanel*[ eventPanelsTotal ];
		for( int i=0; i<eventPanelsTotal; i++ )
		{
			eventPanels[ i ] = NULL;
		}
	}
	
	void checkEventPanels ( float eventPosition )
	{
		float range = 0.05;
		
		for( int i=0; i<eventPanelsTotal; i++ )
		{
			const EventDataItem& dataItem = data->at( i );
			float p = model->getYearAsProgress( dataItem.year );
			
			if( p > ( eventPosition - range * 0.5 ) && p < ( eventPosition + range * 0.5 ) )
			{
				if( eventPanels[ i ] == NULL )		// no event panels.
				{
					EventPanel* eventPanel;
					eventPanel = new EventPanel();
					eventPanel->populate( dataItem );
					eventPanel->setup();
					
					eventPanels[ i ] = eventPanel;
					
					ofAddListener( eventPanel->eventPanelPressedEvent, this, &EventManager :: eventPanelPressedHandler );
					
					addChild( eventPanel );
				}
			}
			else
			{
				if( eventPanels[ i ] != NULL )		// event panel is present.
				{
					EventPanel* eventPanel;
					eventPanel = eventPanels[ i ];
					
					ofRemoveListener( eventPanel->eventPanelPressedEvent, this, &EventManager :: eventPanelPressedHandler );
					
					removeChild( eventPanel );
					
					delete eventPanel;
					eventPanel = NULL;
					
					eventPanels[ i ] = NULL;
				}
			}
		}
	}
	
	void eventPanelPressedHandler ( int& eventID )
	{
		selectedEventID = eventID;
		
		model->setTimelinePlay( false );	// stop the timeline from playing.
		
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
		
		const EventDataItem& data = model->getEventDataItemByEventID( selectedEventID );
		
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
	
	void initElninoEvent ()
	{
		killEvent();
	
		eventItem = new EventElninoItem();
		eventItem->setup();
		eventItem->show();
		
		ofAddListener( eventItem->closeEvent, this, &EventManager :: eventItemCloseHandler );
		ofAddListener( ofEvents.mousePressed, this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void initFutureEvent ()
	{
		killEvent();
		
		eventItem = new EventFutureItem();
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
		
		eventItem->hide();
		
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