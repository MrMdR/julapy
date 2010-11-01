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
#include "SoundManager.h"
#include "InteractionTracker.h"

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
	SoundManager*			sound;
	InteractionTracker*		tracker;
	
	vector<EventDataItem>*	data;
	int						eventID;
	int						selectedEventID;
	bool					bSavedPlayState;
	
	EventPanel**		eventPanels;
	int					eventPanelsTotal;
	vector<ofPoint>		eventPanelPoints;
	vector<bool>		eventPanelPointsAvail;
	
	EventItem*			eventItem;
	EventCover*			eventCover;
	
	//==================================================
	
	void setup ()
	{
		eventItem		= NULL;
		eventCover		= NULL;
		
		eventID			= -1;
		selectedEventID	= -1;
		
		model	= Model :: getInstance();
		sound	= SoundManager :: getInstance();
		tracker	= InteractionTracker :: getInstance();
		
		data			= model->getEventData();
		bSavedPlayState	= model->getTimelinePlay();

		initEventPanels();
		addListeners();
	}
	
	void addListeners ()
	{
		ofAddListener( model->eventProgressChangeEvent,	this, &EventManager :: eventProgressChangeEventHandler );
		ofAddListener( model->futureBtnPressedEvent,	this, &EventManager :: futureBtnPressedEventHandler );
		ofAddListener( model->elninoBtnPressedEvent,	this, &EventManager :: elninoBtnPressedEventHandler );
	}
	
	void removeListeners ()
	{
		ofRemoveListener( model->eventProgressChangeEvent,	this, &EventManager :: eventProgressChangeEventHandler );
		ofRemoveListener( model->futureBtnPressedEvent,		this, &EventManager :: futureBtnPressedEventHandler );
		ofRemoveListener( model->elninoBtnPressedEvent,		this, &EventManager :: elninoBtnPressedEventHandler );
	}
	
	//==================================================
	
	void eventProgressChangeEventHandler ( float& eventPosition )
	{
		checkEventPanels( eventPosition );
	}
	
	//==================================================
	
	void futureBtnPressedEventHandler ( int& eventID )
	{
		bSavedPlayState	= model->getTimelinePlay();
		
		model->setTimelinePlay( false );	// stop the timeline from playing.
		sound->stopAllSoundsButLoop();
		tracker->stopNoInteractionTimeout();
		
		disableEventPanels();
		removeListeners();
		
		initCover();
		initFutureEvent();
	}
	
	//==================================================
	
	void elninoBtnPressedEventHandler ( int& btnID )
	{
		bSavedPlayState	= model->getTimelinePlay();
		
		model->setTimelinePlay( false );	// stop the timeline from playing.
		sound->stopAllSoundsButLoop();
		tracker->stopNoInteractionTimeout();
		
		disableEventPanels();
		removeListeners();
		
		initCover();
		initElninoEvent();
	}
	
	//==================================================
	
	void eventPanelPressedHandler ( int& eventID )
	{
		selectedEventID = eventID;
		
		bSavedPlayState	= model->getTimelinePlay();
		
		model->setTimelinePlay( false );	// stop the timeline from playing.
		sound->stopAllSoundsButLoop();
		tracker->stopNoInteractionTimeout();
		
		disableEventPanels();
		removeListeners();
		
		initCover();
		initEvent();
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
		
		eventPanelPointsAvail.push_back( true );
		eventPanelPoints.push_back( ofPoint() );
		eventPanelPoints.back().x = 330;
		eventPanelPoints.back().y = 199;
		
		eventPanelPointsAvail.push_back( true );
		eventPanelPoints.push_back( ofPoint() );
		eventPanelPoints.back().x = 221;
		eventPanelPoints.back().y = 494;
		
		eventPanelPointsAvail.push_back( true );
		eventPanelPoints.push_back( ofPoint() );
		eventPanelPoints.back().x = 561;
		eventPanelPoints.back().y = 573;
		
		eventPanelPointsAvail.push_back( true );
		eventPanelPoints.push_back( ofPoint() );
		eventPanelPoints.back().x = 1059;
		eventPanelPoints.back().y = 565;

		eventPanelPointsAvail.push_back( true );
		eventPanelPoints.push_back( ofPoint() );
		eventPanelPoints.back().x = 1009;
		eventPanelPoints.back().y = 203;
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
					ofPoint latLon;
					latLon.x = dataItem.lon;
					latLon.y = ABS( dataItem.lat );		// made values positive for simplicity.	
					
					ofPoint mapPoint;
					mapPoint = model->convertEarthPointToPixelPoint( latLon );

					int posIndex;
					posIndex = getClosestEventPanelPointIndex( mapPoint );
					
					eventPanelPointsAvail[ posIndex ] = false;
					
					ofPoint pos;
					pos = eventPanelPoints[ posIndex ];
					
					EventPanel* eventPanel;
					eventPanel = new EventPanel();
					eventPanel->x = pos.x;
					eventPanel->y = pos.y;
					eventPanel->populate( dataItem );
					eventPanel->setPanelPositionIndex( posIndex );
					eventPanel->setup();
					eventPanel->transitionIn();
					
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
					
					int posIndex;
					posIndex = eventPanel->getPanelPositionIndex();
					eventPanelPointsAvail[ posIndex ] = true;
					
					ofRemoveListener( eventPanel->eventPanelPressedEvent, this, &EventManager :: eventPanelPressedHandler );
					
					removeChild( eventPanel );
					
					delete eventPanel;
					eventPanel = NULL;
					
					eventPanels[ i ] = NULL;
				}
			}
		}
	}
	
	int getClosestEventPanelPointIndex ( const ofPoint& p1 )
	{
		int j = 0;
		float dist = 9999999;
		
		for( int i=0; i<eventPanelPoints.size(); i++ )
		{
			if( !eventPanelPointsAvail[ i ] )
				continue;
			
			ofPoint p2;
			p2 = eventPanelPoints[ i ];
			p2.y -= 90;						// middle of the panels.
			
			float d = ofDist( p1.x, p1.y, p2.x, p2.y );
			
			if( d < dist )
			{
				dist = d;
				
				j = i;
			}
		}
		
		return j;
	}
	
	void enableEventPanels ()
	{
		EventPanel* eventPanel;
		
		for( int i=0; i<eventPanelsTotal; i++ )
		{
			eventPanel = eventPanels[ i ];
			
			if( eventPanel != NULL )
			{
				eventPanel->enable();
			}
		}
	}
	
	void disableEventPanels ()
	{
		EventPanel* eventPanel;
		
		for( int i=0; i<eventPanelsTotal; i++ )
		{
			eventPanel = eventPanels[ i ];
			
			if( eventPanel != NULL )
			{
				eventPanel->disable();
			}
		}
	}
	
	//==================================================
	
	void initCover ()
	{
		killCover();
		
		eventCover = new EventCover();
		eventCover->transitionIn();
		
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
	
	//==================================================
	
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
		eventItem->transitionIn();
		
		ofAddListener( eventItem->closeEvent,			this, &EventManager :: eventItemCloseHandler );
		ofAddListener( eventItem->finishedEvent,		this, &EventManager :: eventItemFinishedHandler );
		ofAddListener( eventItem->playOutCompleteEvent,	this, &EventManager :: playOutCompleteHandler );
		ofAddListener( ofEvents.mousePressed,			this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void initElninoEvent ()
	{
		killEvent();
	
		eventItem = new EventElninoItem();
		eventItem->setup();
		eventItem->show();
		eventItem->transitionIn();
		
		ofAddListener( eventItem->closeEvent,			this, &EventManager :: eventItemCloseHandler );
		ofAddListener( eventItem->finishedEvent,		this, &EventManager :: eventItemFinishedHandler );
		ofAddListener( eventItem->playOutCompleteEvent,	this, &EventManager :: playOutCompleteHandler );
		ofAddListener( ofEvents.mousePressed,			this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void initFutureEvent ()
	{
		killEvent();
		
		eventItem = new EventFutureItem();
		eventItem->setup();
		eventItem->show();
		eventItem->transitionIn();
		
		ofAddListener( eventItem->closeEvent,			this, &EventManager :: eventItemCloseHandler );
		ofAddListener( eventItem->finishedEvent,		this, &EventManager :: eventItemFinishedHandler );
		ofAddListener( eventItem->playOutCompleteEvent,	this, &EventManager :: playOutCompleteHandler );
		ofAddListener( ofEvents.mousePressed,			this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void killEvent ()
	{
		if( eventItem == NULL )
			return;
		
		eventItem->hide();
		
		removeChild( eventItem );
		
		ofRemoveListener( eventItem->closeEvent,			this, &EventManager :: eventItemCloseHandler );
		ofRemoveListener( eventItem->finishedEvent,			this, &EventManager :: eventItemFinishedHandler );
		ofRemoveListener( eventItem->playOutCompleteEvent,	this, &EventManager :: playOutCompleteHandler );
		ofRemoveListener( ofEvents.mousePressed,			this, &EventManager :: mousePressedHandler );
		
		delete eventItem;
		eventItem = NULL;
	}

	//==================================================
	
	void playOut ()
	{
		cout << "playOut" << endl;
		
		eventCover->transitionOut();
		eventItem->transitionOut();
	}
	
	void playOutCompleteHandler ( bool& b )
	{
		cout << "playOutCompleteHandler" << endl;
		
		cleanUp();
	}
	
	void cleanUp ()
	{
		killCover();
		killEvent();
		
		enableEventPanels();
		addListeners();
		
		sound->playInstructionalSound();
		tracker->startNoInteractionTimeout();
		model->setTimelinePlay( bSavedPlayState );
	}
	
	//==================================================
	
	void eventItemCloseHandler ( int& eventID )
	{
//		playOut();
		cleanUp();
	}
	
	void eventItemFinishedHandler ( bool& b )
	{
//		playOut();
		cleanUp();
	}
	
	void mousePressedHandler ( ofMouseEventArgs &e )
	{
//		playOut();
		cleanUp();
	}
	
	//==================================================
	
	void draw ()
	{
		//
	}
};