/*
 *  ofxTrackball.cpp
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 14/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxTrackball.h"

ofxTrackball :: ofxTrackball()
{
	reset();
}

ofxTrackball :: ~ofxTrackball()
{

}

void ofxTrackball :: setCenter ( int x, int y )
{
	center.x = x;
	center.y = y;
}

void ofxTrackball :: mouseDown ( int x, int y )
{
	isMouseDown = true;
	
	mouseCurr.x = x;
	mouseCurr.y = y;
}

void ofxTrackball :: mouseUp ( int x, int y )
{
	isMouseDown = false;
}

void ofxTrackball :: mouseMoved ( int x, int y )
{
	mousePrev.x = mouseCurr.x;
	mousePrev.y = mouseCurr.y;
	
	mouseCurr.x = x;
	mouseCurr.y = y;
	
	float mp[ 2 ];
	float mc[ 2 ];
	float cp[ 2 ];
	
	cp[ 0 ] = center.x;
	cp[ 1 ] = center.y;
	
	if( isMouseDown )
	{
		mp[ 0 ] = mousePrev.x;
		mp[ 1 ] = mousePrev.y;
		
		mc[ 0 ] = mouseCurr.x;
		mc[ 1 ] = mouseCurr.y;
		
		mouseVel = mouseCurr - mousePrev;
		
		rollToTrackballSingle( cp, mc, mp, worldRot );
	}
}

void ofxTrackball :: update ()
{
	float mp[ 2 ];
	float mc[ 2 ];
	float cp[ 2 ];
	
	if( isMouseDown )
		return;
	
	if( mouseVel.length() == 0 )
		return;
	
	mouseVel *= 0.95;
	
	if( mouseVel.length() < 0.01 )
	{
		mouseVel.set( 0, 0 );
		return;
	}
	
	cp[ 0 ] = center.x;
	cp[ 1 ] = center.y;
	
	mp[ 0 ] = mouseCurr.x;
	mp[ 1 ] = mouseCurr.y;
	
	mc[ 0 ] = mouseCurr.x + mouseVel.x;
	mc[ 1 ] = mouseCurr.y + mouseVel.y;
	
	rollToTrackballSingle( cp, mc, mp, worldRot );	
}

void ofxTrackball :: rotate ()
{
	glRotatef( trackRot[ 0 ], trackRot[ 1 ], trackRot[ 2 ], trackRot[ 3 ] );
	glRotatef( worldRot[ 0 ], worldRot[ 1 ], worldRot[ 2 ], worldRot[ 3 ] );
}

void ofxTrackball :: reset ()
{
	trackRot[ 0 ] = 0;
	trackRot[ 1 ] = 0;
	trackRot[ 2 ] = 0;
	trackRot[ 3 ] = 0;
	
	worldRot[ 0 ] = 0;
	worldRot[ 1 ] = 0;
	worldRot[ 2 ] = 0;
	worldRot[ 3 ] = 0;
	
	mouseCurr.set( 0, 0 );
	mousePrev.set( 0, 0 );
	mouseVel.set( 0, 0 );
	
	isMouseDown	= false;
}