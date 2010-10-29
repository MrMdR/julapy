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
		model->setTimelinePlay( false );	// stop the timeline from playing.
		model->stopAllSoundsButLoop();
		
		initCover();
		initFutureEvent();
	}
	
	//==================================================
	
	void elninoBtnPressedEventHandler ( int& btnID )
	{
		model->setTimelinePlay( false );	// stop the timeline from playing.
		model->stopAllSoundsButLoop();
		
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
	
	void eventPanelPressedHandler ( int& eventID )
	{
		selectedEventID = eventID;
		
		model->setTimelinePlay( false );	// stop the timeline from playing.
		model->stopAllSoundsButLoop();
		
		initCover();
		initEvent();
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
		
		ofAddListener( eventItem->closeEvent,		this, &EventManager :: eventItemCloseHandler );
		ofAddListener( eventItem->finishedEvent,	this, &EventManager :: eventItemFinishedHandler );
		ofAddListener( ofEvents.mousePressed,		this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void initElninoEvent ()
	{
		killEvent();
	
		eventItem = new EventElninoItem();
		eventItem->setup();
		eventItem->show();
		eventItem->transitionIn();
		
		ofAddListener( eventItem->closeEvent,		this, &EventManager :: eventItemCloseHandler );
		ofAddListener( eventItem->finishedEvent,	this, &EventManager :: eventItemFinishedHandler );
		ofAddListener( ofEvents.mousePressed,		this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void initFutureEvent ()
	{
		killEvent();
		
		eventItem = new EventFutureItem();
		eventItem->setup();
		eventItem->show();
		eventItem->transitionIn();
		
		ofAddListener( eventItem->closeEvent,		this, &EventManager :: eventItemCloseHandler );
		ofAddListener( eventItem->finishedEvent,	this, &EventManager :: eventItemFinishedHandler );
		ofAddListener( ofEvents.mousePressed,		this, &EventManager :: mousePressedHandler );
		
		addChild( eventItem );
	}
	
	void killEvent ()
	{
		if( eventItem == NULL )
			return;
		
		eventItem->hide();
		
		removeChild( eventItem );
		
		ofRemoveListener( eventItem->closeEvent,	this, &EventManager :: eventItemCloseHandler );
		ofRemoveListener( eventItem->finishedEvent,	this, &EventManager :: eventItemFinishedHandler );
		ofRemoveListener( ofEvents.mousePressed,	this, &EventManager :: mousePressedHandler );
		
		delete eventItem;
		eventItem = NULL;
		
		playInstructionalSound();
	}
	
	void eventItemCloseHandler ( int& eventID )
	{
		killCover();
		killEvent();
	}
	
	void eventItemFinishedHandler ( bool& b )
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
	
	void playInstructionalSound ()
	{
		if( ofRandom( 0, 1 ) > 0.7 )		// not every time.
		{
			ofSoundPlayer* sound;
			if( ofRandom( 0, 1 ) > 0.4 )
			{
				sound = ofxAssets :: getInstance()->getSound( "instruct_1" );
			}
			else
			{
				sound = ofxAssets :: getInstance()->getSound( "instruct_0" );
			}
			
			if( sound->getPosition() == 0 )		// only play if its not playing.
			{
				sound->setPosition( 0 );
				sound->play();
			}
		}
	}
	
	//==================================================
	
	void draw ()
	{
		//
	}
};