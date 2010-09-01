/*
 *  FooterBar.h
 *  iPhoneEmptyExample
 *
 *  Created by Lukasz Karluk on 19/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "FooterBarViewController.h"

class FooterBar
{

public:
	
	FooterBar()
	{
		ofAddListener( ofEvents.update, this, &FooterBar :: update	);
	};
	
	~FooterBar()
	{
		//
	};
	
	//----------------------------------------
	
	FooterBarViewController*	controller;
	
	//----------------------------------------
	
	void setup ()
	{
		CGRect appRect = [ [ UIScreen mainScreen ] applicationFrame ];
		
		controller = [ [ FooterBarViewController alloc ] init ];
		controller.view.frame = appRect;
		
		UIWindow* window = ofxiPhoneGetUIWindow();
		[ window addSubview: controller.view ];
		[ controller.view release ];
	};
	
	void show ()
	{
		[ controller show ];
	};
	
	void hide ()
	{
		[ controller hide ];
	};
	
	void toggleShow ()
	{
		[ controller toggleShow ];
	};
	
private:
	
	void update ( ofEventArgs &e )
	{
		// checks.
	};
	
};