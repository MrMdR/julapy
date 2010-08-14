/*
 *  ColorCycle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorCycle.h"

ColorCycle :: ColorCycle ()
{
	setScreenSize( ofGetWidth(), ofGetHeight() );
	setFrameRate( ofGetFrameRate() );
	showPanel();
}

ColorCycle :: ~ColorCycle ()
{
	//
}

void ColorCycle :: setScreenSize ( int w, int h )
{
	screen.set( w, h );
}

void ColorCycle :: setFrameRate	( int fr )
{
	frameRate = fr;
}

void ColorCycle :: setup ()
{
	colorPicker0.setColorRadius( 1.0 );
	colorPicker1.setColorRadius( 1.0 );
	
	colorPicker0.setColorAngle( 0 );
	colorPicker1.setColorAngle( 1 / 3.0 );
	
	colorScale = 1.0;
	
	bInputDown = false;
}

void ColorCycle :: update ()
{
	if( bInputDown )
	{
		float velx = inputPos1.x - inputPos2.x;
		float vely = inputPos1.y - inputPos2.y;
		
		inputVel.x += ( velx - inputVel.x ) * 0.1;
		inputVel.y += ( vely - inputVel.y ) * 0.1;
	}
	else
	{
		float ease = 0.98;
		inputVel.x *= ease;
		inputVel.y *= ease;
	}
	
	float ang;
	ang = inputVel.y / 10000.0;
	ang += frameRate * 0.00001;
	
	colorPicker0.addToColorAngle( ang );
	colorPicker1.addToColorAngle( ang );
	
	colorScale += ( inputVel.x / 10000.0 );
	colorScale = MAX( MIN( colorScale, 1.0 ), 0.5 );
	
	colorPicker0.setColorScale( colorScale );
	colorPicker1.setColorScale( colorScale );
	
	rect.setCornerColor( colorPicker0.getColor(), 0 );
	rect.setCornerColor( colorPicker0.getColor(), 1 );
	rect.setCornerColor( colorPicker1.getColor(), 2 );
	rect.setCornerColor( colorPicker1.getColor(), 3 );
}

void ColorCycle :: draw ()
{
	rect.draw();
	
	drawColorPickers();
}

void ColorCycle :: drawColorPickers ()
{
	if( !bPanel )
		return;
	
	int s  = screen.screenHeight * 0.209;
	int py = 30;
	int px = 30;
	
	colorPicker0.draw( px, py, s, s );
	
	py += colorPicker0.getHeight() + 10;
	
	colorPicker1.draw( px, py, s, s );
}

///////////////////////////////////////////////////////
//	PANEL.
///////////////////////////////////////////////////////

void ColorCycle :: showPanel ()
{
	bPanel = true;
}

void ColorCycle ::  hidePanel ()
{
	bPanel = false;
}

void ColorCycle ::  togglePanel	()
{
	bPanel = !bPanel;
}


///////////////////////////////////////////////////////
//	INPUT.
///////////////////////////////////////////////////////

void ColorCycle :: down ( int x, int y )
{
	bInputDown = true;
	
	inputPos1.x = inputPos2.x = x;
	inputPos1.y = inputPos2.y = y;
}

void ColorCycle :: drag ( int x, int y )
{
	inputPos2.x = inputPos1.x;	// copy old values.
	inputPos2.y = inputPos1.y;
	
	inputPos1.x = x;			// save new values.
	inputPos1.y = y;
}

void ColorCycle :: up ( int x, int y )
{
	bInputDown = false;
}
