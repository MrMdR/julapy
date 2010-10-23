/*
 *  EventPanelManager.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 23/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxFlash.h"

#include "Model.h"

#include "EventPanel.h"

class EventPanelManager : public ofxSprite
{

public:
	
	EventPanelManager()
	{
		setup();
	}
	
	~EventPanelManager(){}
	
	//==================================================
	
	Model*			model;
	int				eventID;
	EventPanel*		eventPanel;
	
	//==================================================
	
	void setup ()
	{
		eventPanel = NULL;
		
		model = Model :: getInstance();
		
		ofAddListener( model->timelineMarkerPressedEvent, this, &EventPanelManager :: timelineMarkerPressedEventHandler );
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
		
		addChild( eventPanel );
	}
	
	void killEventPanel ()
	{
		if( eventPanel == NULL )
			return;
		
		removeChild( eventPanel );
		
		delete eventPanel;
		eventPanel = NULL;
	}
};