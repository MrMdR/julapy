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
#include "Quad.h"

class TimelineTabBtn : public ofxSprite
{

public:
	
	TimelineTabBtn( int btnId = -1 )
	{
		setID( btnId );
		
		alpha		= 0;
		visible		= false;
		
		bShow		= false;
		bAnimating	= false;
		bPlayedIn	= false;
		bPlayedOut	= false;
		
		startTime	= 0;
		endTime		= 0;
		animTime	= 0.14;
	}
	
	~TimelineTabBtn() {};
	
	//==================================================
	
	ofImage*	icon;
	ofImage*	black;
	Btn			btn;
	int			btnId;
	
	bool		bShow;
	bool		bAnimating;
	bool		bPlayedIn;
	bool		bPlayedOut;
	float		startTime;
	float		endTime;
	float		animTime;
	
	ofEvent<int> btnPressEvent;
	
	//==================================================
	
	void setup ()
	{
		ofAddListener( btn.btnPressEvent,	this, &TimelineTabBtn :: btnPressed );
	}
	
	void setPos ( int x, int y )
	{
		this->x = x;
		this->y = y;
		
		btn.setPos( x, y );
	}
	
	void setSize ( int w, int h )
	{
		width  = w;
		height = h;
		
		btn.setSize( w, h );
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
//		visible = true;
//		return;
		
		if( bShow )
			return;
		
		bShow		= true;
		bAnimating	= true;
		
		visible		= true;
		
		startTime	= ofGetElapsedTimef();
		endTime		= startTime + animTime;
	}
	
	void hide ()
	{
//		visible = false;
//		return;
		
		if( !bShow )
			return;
		
		bShow		= false;
		bAnimating	= true;
		
		startTime	= ofGetElapsedTimef();
		endTime		= startTime + animTime;
	}
	
	void update ()
	{
//		return;
		
		if( !bAnimating )
			return;
		
		float t = ( ofGetElapsedTimef() - startTime ) / animTime;
		if( t > 1.0 )
		{
			t = 1.0;
			visible		= bShow;
			bAnimating	= false;
			
			if( bShow )
			{
				bPlayedIn = true;
			}
			else
			{
				bPlayedOut = true;
			}
		}
		
		float v = Quad :: easeOut( t, 0, 1.0, 1.0 );
		
		if( bShow )
		{
			alpha = v;
		}
		else 
		{
			alpha = 1 - v;
		}
	}

	void draw ()
	{
		ofEnableAlphaBlending();
		
		ofSetColor( 255, 255, 255, 255 * ( 1 - alpha ) );
		black->draw( x, y );
		
//		if( !visible )
//			black->draw( x, y );
		
		ofSetColor( 0xFFFFFF );
		icon->draw( x, y );
		
		ofDisableAlphaBlending();
	}
	
	//==================================================

	void btnPressed ( int & btnId )
	{
		ofNotifyEvent( btnPressEvent, this->btnId, this );
	}

};