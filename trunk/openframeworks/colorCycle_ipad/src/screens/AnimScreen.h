/*
 *  AnimScreen.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 29/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

class AnimScreen
{

public:
	
	AnimScreen ()
	{
		alpha		= 0;
		bShow		= false;
		bVisible	= false;
		bAnimating	= false;
		startTime	= 0;
		endTime		= 0;
		animTime	= 0.2;
		
		ofAddListener( ofEvents.update, this, &AnimScreen :: update );
	}
	
	//---------------------------------
	
	int		alpha;
	bool	bShow;
	bool	bVisible;
	bool	bAnimating;
	float	startTime;
	float	endTime;
	float	animTime;
	
	//---------------------------------
	
	virtual void toggle ()
	{
		if( bShow )
		{
			hide();
		}
		else
		{
			show();
		}
	}
	
	virtual void show ()
	{
		if( bShow )
			return;
		
		bShow		= true;
		bVisible	= true;
		bAnimating	= true;
		startTime	= ofGetElapsedTimef();
		endTime		= startTime + animTime;
	}
	
	virtual void hide ()
	{
		if( !bShow )
			return;
		
		bShow		= false;
		bAnimating	= true;
		startTime	= ofGetElapsedTimef();
		endTime		= startTime + animTime;
	}
	
	virtual void update ( ofEventArgs &e )
	{
		update();
	}
	
	virtual void update ()
	{
		if( !bAnimating )
			return;
		
		float t = ( ofGetElapsedTimef() - startTime ) / animTime;
		if( t > 1.0 )
		{
			t = 1.0;
			bVisible	= bShow;
			bAnimating	= false;
		}
		
		float v = Quad :: easeOut( t, 0, 1.0, 1.0 );
		
		if( bShow )
		{
			alpha = v * 255;
		}
		else 
		{
			alpha = ( 1 - v ) * 255;
		}
	}
};