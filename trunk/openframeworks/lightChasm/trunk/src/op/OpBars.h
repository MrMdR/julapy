/*
 *  OpBars.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 9/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofFBOTexture.h"

#define BARS_MAX	20

class OpBars
{
	
public :
	
	OpBars();
	~OpBars();

	void init( int w = 640, int h = 480 );
	void setNumberOfBars( int value = BARS_MAX );
	int  getNumberOfBars();
	void setAudioInData( float *data );
	void setAudioAvgMin( float avgMin );
	void update();

	void drawToFBO();
	void drawFBOToScreen();
	void draw();
	
	vector <ofRectangle> bars;
	
private :
	
	ofFBOTexture fbo;

	int		width;
	int		height;
	int		barsTotal;
	float	audioAvgMin;
	float	audioAverage[ BARS_MAX ];
	float	audioPeak[ BARS_MAX ];

};