/*
 *  ClockInfoScreen.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 31/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLOCK_INFO_SCREEN_H
#define CLOCK_INFO_SCREEN_H

#include "ofMain.h"

class ClockInfoScreen
{
public :
	
	 ClockInfoScreen();
	~ClockInfoScreen();
	
	void setup		();
	void update		();
	void draw		();
	
	void show		();
	void hide		();
	
	bool	bShowing;
	int		screenWidth;
	int		screenHeight;
};

#endif