/*
 *  OpBars.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 9/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

#define BARS_MAX	20

class OpBars
{
	
public :
	
	OpBars();
	~OpBars();

	void init( int w = 640, int h = 480 );
	void setNumberOfBars( int value = BARS_MAX );
	void setAudioInData( float *data );
	void update();
	void draw();
	
	vector<ofRectangle>	bars;
	
private :

	int		width;
	int		height;
	int		barsTotal;
	float	audioInData[ BARS_MAX ];

};