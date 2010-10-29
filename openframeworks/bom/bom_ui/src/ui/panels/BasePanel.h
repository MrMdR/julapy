/*
 *  BasePanel.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 29/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxFlash.h"
#include "Quad.h"

class BasePanel : public ofxSprite
{

public:
	
	BasePanel()
	{
		alpha		= 0;
		visible		= false;
		
		bShow		= false;
		bAnimating	= false;
		bPlayedIn	= false;
		bPlayedOut	= false;
		
		startTime	= 0;
		endTime		= 0;
		animTime	= 0.2;
	}
	
	~BasePanel()
	{
		
	}
	
	//==================================================
	
	bool	bShow;
	bool	bAnimating;
	bool	bPlayedIn;
	bool	bPlayedOut;
	float	startTime;
	float	endTime;
	float	animTime;
	
	//==================================================
	
	virtual void transitionIn ()
	{
		if( bShow )
			return;
		
		bShow		= true;
		bAnimating	= true;
		
		visible		= true;
		
		startTime	= ofGetElapsedTimef();
		endTime		= startTime + animTime;
	}
	
	virtual void transitionOut ()
	{
		if( !bShow )
			return;
		
		bShow		= false;
		bAnimating	= true;
		
		startTime	= ofGetElapsedTimef();
		endTime		= startTime + animTime;
	}
	
	virtual void update ()
	{
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
};