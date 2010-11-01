/*
 *  InteractionTimer.h
 *  bom
 *
 *  Created by lukasz karluk on 30/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "SoundManager.h"
#include "Model.h"

class InteractionTracker
{
	
private: 
	
	static InteractionTracker* _instance;
	
	InteractionTracker() 
	{
		ofAddListener( ofEvents.mousePressed,	this, &InteractionTracker :: _mousePressed	);
		ofAddListener( ofEvents.mouseMoved,		this, &InteractionTracker :: _mouseMoved	);
		ofAddListener( ofEvents.mouseDragged,	this, &InteractionTracker :: _mouseDragged	);
		ofAddListener( ofEvents.mouseReleased,	this, &InteractionTracker :: _mouseReleased	);
		ofAddListener( ofEvents.update,			this, &InteractionTracker :: _update		);
		
		init();
	}
	
	~InteractionTracker()
	{
		//
	}
	
	//==================================================
	
	SoundManager*	sound;
	Model*			model;
	
	float	noInteractionTimeoutStart;
	float	noInteractionTimeoutRunning;
	float	noInteractionTimeoutLimit;
	bool	noInteractionTimeoutPaused;
	
	//==================================================
	
	void init ()
	{
		sound = SoundManager :: getInstance();
		model = Model :: getInstance();
		
		resetNoInteractionTimeout();
	}
	
	void _mousePressed( ofMouseEventArgs &e )
	{
		resetNoInteractionTimeout();
	}

	void _mouseMoved( ofMouseEventArgs &e )
	{
		//
	}

	void _mouseDragged( ofMouseEventArgs &e )
	{
		//
	}

	void _mouseReleased( ofMouseEventArgs &e )
	{
		//
	}
	
	void _update ( ofEventArgs &e )
	{
		checkNoInteractionTimeout();
	}
	
public:
	
	static InteractionTracker* getInstance()
	{
		if( !_instance )
		{
			_instance = new InteractionTracker();
		}
		
        return _instance;
	}

	//==================================================
	
	void startNoInteractionTimeout ()
	{
		resetNoInteractionTimeout();
	}
	
	void stopNoInteractionTimeout ()
	{
		resetNoInteractionTimeout();
		
		noInteractionTimeoutPaused	= true;
	}
	
	void resetNoInteractionTimeout ()
	{
		noInteractionTimeoutStart	= ofGetElapsedTimef();
		noInteractionTimeoutRunning	= 0;
		noInteractionTimeoutLimit	= 20;
		noInteractionTimeoutPaused	= false;
	}
	
	void checkNoInteractionTimeout ()
	{
		if( noInteractionTimeoutPaused )
			return;
		
		float elapsedTime;
		elapsedTime = ofGetElapsedTimef();
		
		float noInteractionTimeoutChange;
		noInteractionTimeoutChange = elapsedTime - ( noInteractionTimeoutStart + noInteractionTimeoutRunning );
		
		noInteractionTimeoutRunning += noInteractionTimeoutChange;
		
//		cout << noInteractionTimeoutRunning << endl;
		
		if( noInteractionTimeoutRunning > noInteractionTimeoutLimit )
		{
			sound->playIdle();
			model->setTimelinePlay( true );
			
			resetNoInteractionTimeout();
		}
	}
};
