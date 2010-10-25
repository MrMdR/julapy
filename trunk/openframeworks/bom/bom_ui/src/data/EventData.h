/*
 *  EventData.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 16/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "EventDataItem.h"

#define EVENT_TYPE_IMAGE	0
#define EVENT_TYPE_VIDEO	1
#define EVENT_TYPE_TEXT		2

class EventData
{

public:
	
	 EventData() {};
	~EventData() {};
	
	//==================================================
	
	ofxXmlSettings			xml;
	vector<EventDataItem>	items;
	
	//==================================================
	
	void loadFile ( string xmlFile )
	{
		xml.loadFile( xmlFile );
		
		xml.pushTag( "events", 0 );
		int numOfEvents = xml.getNumTags( "event" );
		
		for( int i=0; i<numOfEvents; i++ )
		{
			EventDataItem item;
			item.id		= i;
			item.title	= xml.getValue( "event:title",	"",		i );
			item.year	= xml.getValue( "event:year",	1915,	i );
			item.lat	= xml.getValue( "event:lat",	120.0,	i );
			item.lon	= xml.getValue( "event:lon",	55.0,	i );
			item.thumb	= xml.getValue( "event:thumb",	"",		i );
			item.image	= xml.getValue( "event:image",	"",		i );
			item.video	= xml.getValue( "event:video",	"",		i );
			item.sound	= xml.getValue( "event:sound",	"",		i );
			item.body	= xml.getValue( "event:body",	"",		i );
			
			if( item.image.length() > 0 )
			{
				item.type = EVENT_TYPE_IMAGE;
			}
			else if( item.video.length() > 0 )
			{
				item.type = EVENT_TYPE_VIDEO;
			}
			else
			{
				item.type = EVENT_TYPE_TEXT;
			}
			
			items.push_back( item );
		}
	}
};