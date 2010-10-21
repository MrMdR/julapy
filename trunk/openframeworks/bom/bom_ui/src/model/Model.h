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

#define TIMELINE_TAB_RAIN		0
#define TIMELINE_TAB_TEMP		1

#define TIMELINE_PLAY_SPEED		0.001

class Model
{

private: 
	
	static Model* _instance;
	
	Model() 
	{
		tabType			= TIMELINE_TAB_RAIN;
		bTimelinePlay	= false;
	};
	
	~Model() {};
	
	//==================================================
	
	int		tabType;
	bool	bTimelinePlay;
	
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
	
	ofEvent<int>	tabTypeChangeEvent;
	ofEvent<bool>	timelinePlayChangeEvent;
	
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
};