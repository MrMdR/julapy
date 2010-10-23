/*
 *  KeyData.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 23/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "KeyDataItem.h"

class KeyData
{
	
public:
	
	 KeyData() {};
	~KeyData() {};
	
	//==================================================
	
	ofxXmlSettings			xml;
	vector<KeyDataItem>		items;
	
	//==================================================
	
	void loadFile ( string xmlFile )
	{
		xml.loadFile( xmlFile );
		
		xml.pushTag( "keys", 0 );
		int numOfEvents = xml.getNumTags( "key" );
		
		for( int i=0; i<numOfEvents; i++ )
		{
			KeyDataItem item;
			item.color.r	= xml.getValue( "key:color:r",	255, i );
			item.color.g	= xml.getValue( "key:color:g",	255, i );
			item.color.b	= xml.getValue( "key:color:b",	255, i );
			item.color.a	= xml.getValue( "key:color:a",	255, i );
			item.text		= xml.getValue( "key:text",		"",	 i );
			
			items.push_back( item );
		}
	}
};