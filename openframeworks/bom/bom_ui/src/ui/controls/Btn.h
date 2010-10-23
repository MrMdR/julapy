/*
 *  Button.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 20/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"
#include "ofxMSAInteractiveObject.h"

class Btn : public ofxMSAInteractiveObject
{

public:
	
	Btn( int btnId = -1 )
	{
		setID( btnId );
		
		disableAppEvents();
		enableMouseEvents();
	};
	
	~Btn()
	{
		disableAllEvents();
	};
	
	//==================================================
	
	int btnId;
	
	//==================================================
	
	void setID ( int btnId )
	{
		this->btnId = btnId;
	}
	
	int getID ()
	{
		return btnId;
	}
	
	virtual void onRollOver ( int x, int y )
	{
		ofNotifyEvent( btnOverEvent, btnId, this );
	}
	
	virtual void onRollOut ()
	{
		ofNotifyEvent( btnOutEvent, btnId, this );
	}
	
	virtual void onPress ( int x, int y, int button )
	{
		ofNotifyEvent( btnPressEvent, btnId, this );
	}
	
	ofEvent<int> btnOverEvent;
	ofEvent<int> btnOutEvent;
	ofEvent<int> btnPressEvent;
	
};