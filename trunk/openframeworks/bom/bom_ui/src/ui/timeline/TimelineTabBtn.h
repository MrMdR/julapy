/*
 *  TimelineTabBtn.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 20/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlash.h"
#include "Btn.h"

class TimelineTabBtn : public ofxSprite
{

public:
	
	TimelineTabBtn( int btnId = -1 )
	{
		setID( btnId );
	}
	
	~TimelineTabBtn() {};
	
	//==================================================
	
	ofImage*	icon;
	ofImage*	black;
	Btn*		btn;
	int			btnId;
	
	//==================================================
	
	void setup ()
	{
		btn = new Btn();
		
		ofAddListener( btn->btnOverEvent,	this, &TimelineTabBtn :: btnOver );
		ofAddListener( btn->btnOutEvent,	this, &TimelineTabBtn :: btnOut );
		ofAddListener( btn->btnPressEvent,	this, &TimelineTabBtn :: btnPressed );
	}
	
	void setPos ( int x, int y )
	{
		this->x = x;
		this->y = y;
		
		btn->setPos( x, y );
	}
	
	void setSize ( int w, int h )
	{
		width  = w;
		height = h;
		
		btn->setSize( w, h );
	}
	
	void setID ( int btnId )
	{
		this->btnId = btnId;
	}
	
	int getID ()
	{
		return btnId;
	}
	
	void setIcon ( ofImage* icon )
	{
		this->icon = icon;
	}
	
	void setBlack ( ofImage* black )
	{
		this->black = black;
	}
	
	void show ()
	{
		visible = true;
	}
	
	void hide ()
	{
		visible = false;
	}

	void draw ()
	{
		ofSetColor( 0xFFFFFF );
		
		ofEnableAlphaBlending();
		
		if( !visible )
		{
			black->draw( x, y );
		}
		
		icon->draw( x, y );
		
		ofDisableAlphaBlending();
	}
	
	//==================================================

	void btnOver ( int & btnId )
	{
		//
	}
	
	void btnOut ( int & btnId )
	{
		//
	}
	
	void btnPressed ( int & btnId )
	{
		ofNotifyEvent( btnPressEvent, this->btnId, this );
	}
	
	ofEvent<int> btnPressEvent;

};