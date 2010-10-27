/*
 *  Model.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 21/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "EventDataItem.h"
#include "KeyDataItem.h"

#define TIMELINE_TAB_RAIN		0
#define TIMELINE_TAB_TEMP		1

#define TIMELINE_PLAY_SPEED		0.001
#define YEAR_START				1915
#define YEAR_END				2005

class Model
{

private: 
	
	static Model* _instance;
	
	Model() 
	{
		tabType			= TIMELINE_TAB_RAIN;
		bTimelinePlay	= false;
		progress		= 0;
		eventProgress	= 0;
		year			= YEAR_START;
	};
	
	~Model() {};
	
	//==================================================

	int						tabType;
	bool					bTimelinePlay;
	float					progress;
	float					eventProgress;
	int						year;
	vector<KeyDataItem>		keyRainData;
	vector<KeyDataItem>		keyTempData;
	ofRectangle				pixelRect;
	ofRectangle				earthRect;
	
	//==================================================
	
public:
	
	static Model* getInstance()
	{
		if( !_instance )
		{
			_instance = new Model();
		}
		
        return _instance;
	}
	
	//==================================================

	vector<EventDataItem>	eventData;
	ofEvent<int>			timelineMarkerPressedEvent;
	ofEvent<int>			tabTypeChangeEvent;
	ofEvent<bool>			timelinePlayChangeEvent;
	ofEvent<int>			futureBtnPressedEvent;
	ofEvent<float>			progressChangeEvent;
	ofEvent<float>			eventProgressChangeEvent;
	ofEvent<int>			yearChangeEvent;

	//==================================================
	
	void setKeyRainData ( const vector<KeyDataItem>& data )
	{
		keyRainData = data;
	}
	
	vector<KeyDataItem>& getKeyRainData ()
	{
		return keyRainData;
	}
	
	void setKeyTempData ( const vector<KeyDataItem>& data )
	{
		keyTempData = data;
	}
	
	vector<KeyDataItem>& getKeyTempData ()
	{
		return keyTempData;
	}
	
	//==================================================
	
	void setEventData ( const vector<EventDataItem>& data )
	{
		for( int i=0; i<data.size(); i++ )
		{
			EventDataItem dataItem = data[ i ];
			
			if( dataItem.year >= YEAR_START && dataItem.year <= YEAR_END )		// exculde any items beyond the timeline period.
			{
				eventData.push_back( dataItem );
			}
			else 
			{
				cout << "event " << i << " exculded - out of date range = " << dataItem.year << endl;
			}

		}
	}
	
	vector<EventDataItem>* getEventData ()
	{
		return &eventData;
	}
	
	EventDataItem getEventDataItemAt ( int i )
	{
		i = ofClamp( i, 0, eventData.size() - 1 );
		
		return eventData[ i ];
	}
	
	int getEventDataSize ()
	{
		return eventData.size();
	}
	
	//==================================================
	
	void setTimelineMarkerPress ( int eventID )
	{
		ofNotifyEvent( timelineMarkerPressedEvent, eventID, this );
	}
	
	//==================================================
	
	void setFutureBtnPress ( int eventID )
	{
		ofNotifyEvent( futureBtnPressedEvent, eventID, this );
	}
	
	//==================================================
	
	void setTabType ( int tabType )
	{
		if( this->tabType == tabType )
			return;
		
		this->tabType = tabType;
		
		ofNotifyEvent( tabTypeChangeEvent, this->tabType, this );
	}
	
	void toggleTabType ()
	{
		if( tabType == TIMELINE_TAB_RAIN )
		{
			setTabType( TIMELINE_TAB_TEMP );
		}
		else if( tabType == TIMELINE_TAB_TEMP )
		{
			setTabType( TIMELINE_TAB_RAIN );
		}
	}
	
	int getTabType ()
	{
		return tabType;
	}
	
	//==================================================
	
	void setTimelinePlay ( bool bTimelinePlay )
	{
		if( this->bTimelinePlay == bTimelinePlay )
			return;
		
		this->bTimelinePlay = bTimelinePlay;
		
		ofNotifyEvent( timelinePlayChangeEvent, this->bTimelinePlay, this );
	}
	
	void toggleTimelinePlay ()
	{
		setTimelinePlay( !bTimelinePlay );
	}
	
	bool getTimelinePlay ()
	{
		return bTimelinePlay;
	}
	
	//==================================================
	
	void setProgress ( float p )
	{
		if( progress == p )
			return;
		
		progress = p;
		
		ofNotifyEvent( progressChangeEvent, this->progress, this );
		
		//-- update year based on progress.
		
		int yr;
		yr = ( YEAR_END - YEAR_START ) * progress + YEAR_START;
		
		setYear( yr );
	}
	
	float getProgress ()
	{
		return progress;
	}
	
	//==================================================
	
	void setEventProgress ( float p )
	{
		p = ( (int)( p * 1000 ) ) / 1000.0;		// round off float to 2 decimal points.
		
		if( eventProgress == p )
			return;
		
		eventProgress = p;
		
		ofNotifyEvent( eventProgressChangeEvent, eventProgress, this );
	}
	
	float getEventProgress ()
	{
		return eventProgress;
	}
	
	//==================================================
	
	void setYear ( int year )
	{
		if( this->year == year )
			return;
		
		this->year = year;
		
		ofNotifyEvent( yearChangeEvent, this->year, this );
	}
	
	int getYear ()
	{
		return year;
	}
	
	float getYearAsProgress ( int year )
	{
		float p = ( year - YEAR_START ) / (float)( YEAR_END - YEAR_START );
		
		return p;
	}
	
	//==================================================
	
	void setPixelRect ( const ofRectangle& rect )
	{
		pixelRect = rect;
	}
	
	void setEarthRect ( const ofRectangle& rect )
	{
		earthRect = rect;
	}

	ofRectangle getPixelRect ()
	{
		return pixelRect;
	}

	ofRectangle getEarthRect ()
	{
		return earthRect;
	}
	
	ofPoint convertEarthPointToPixelPoint( const ofPoint& earthPoint )
	{
		ofPoint p;
		
		float px = ( earthPoint.x - earthRect.x ) / (float)earthRect.width;
		float py = ( earthPoint.y - earthRect.y ) / (float)earthRect.height;
		
		p.x = px * pixelRect.width  + pixelRect.x;
		p.y = py * pixelRect.height + pixelRect.y;
		
		return p;
	}
	
	ofPoint convertPixelPointToEarthPoint( const ofPoint& pixelPoint )
	{
		ofPoint p;

		float px = ( pixelPoint.x - pixelRect.x ) / (float)pixelRect.width;
		float py = ( pixelPoint.y - pixelRect.y ) / (float)pixelRect.height;
		
		p.x = px * earthRect.width  + earthRect.x;
		p.y = py * earthRect.height + earthRect.y;
		
		return p;
	}
	
	//==================================================
};