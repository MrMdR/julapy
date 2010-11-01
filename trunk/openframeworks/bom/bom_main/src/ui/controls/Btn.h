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
	ofEvent<int> btnOverEvent;
	ofEvent<int> btnOutEvent;
	ofEvent<int> btnPressEvent;
	
	//==================================================
	
	void setID ( int btnId )
	{
		this->btnId = btnId;
	}
	
	int getID ()
	{
		return btnId;
	}
	
	void onRollOver ( int x, int y )
	{
		ofNotifyEvent( btnOverEvent, btnId, this );
	}
	
	void onRollOut ()
	{
		ofNotifyEvent( btnOutEvent, btnId, this );
	}
	
	void onPress ( int x, int y, int button )
	{
		ofNotifyEvent( btnPressEvent, btnId, this );
	}
};