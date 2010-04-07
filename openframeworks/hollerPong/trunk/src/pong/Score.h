/*
 *  Score.h
 *  emptyExample
 *
 *  Created by lukasz karluk on 6/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HOLLER_PONG_SCORE
#define HOLLER_PONG_SCORE

#include "ofMain.h"

class Score
{

public :
	
	void init			();
	void addToPlayerOne	( int points );
	void addToPlayerTwo	( int points );
	void reset			();
	void update			();
	void draw			();
	
	int				points[ 2 ];
	ofTrueTypeFont	verdana;
	
};

#endif