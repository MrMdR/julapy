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
	
	white.r = 255;
	white.g = 255;
	white.b = 255;
	
	black.r = 0;
	black.g = 0;
	black.b = 0;
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

void ColorCycle :: setGravity ( float gx, float gy )
{
	physics.setGravity( gx, gy );
}

///////////////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////////////

void ColorCycle :: setup ()
{
	colorPicker0.setColorRadius( 1.0 );
	colorPicker1.setColorRadius( 1.0 );
	
	colorPicker0.setColorAngle( 3 / 6.0 );
	colorPicker1.setColorAngle( 5 / 6.0 );
	
	colorPicker0.disable();
	colorPicker1.disable();
	
	colorScale = 1.0;
	
	bInputDown = false;
	
	physics.setScreen( screen );
	physics.setup();
}

///////////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////////

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
//	ang += frameRate * 0.00001;
	
	colorPicker0.addToColorAngle( ang );
	colorPicker1.addToColorAngle( ang );
	
	colorScale += ( inputVel.x / 10000.0 );
	colorScale = MAX( MIN( colorScale, 1.0 ), 0.5 );
	
	colorPicker0.setColorScale( colorScale );
	colorPicker1.setColorScale( colorScale );
	
	upperColor = colorPicker0.getColor();
	lowerColor = colorPicker1.getColor();
	
	rect.setCornerColor( upperColor, 0 );
	rect.setCornerColor( upperColor, 1 );
	rect.setCornerColor( lowerColor, 2 );
	rect.setCornerColor( lowerColor, 3 );
	
	updatePhysics();
	updateDelaunay();
	updateTriangles();
}

void ColorCycle :: updatePhysics ()
{
	physics.update();
	
	for( int i=0; i<physics.circles.size(); i++ )
	{
		ofColor c;
		c = interpolateColor( upperColor, lowerColor, physics.circles[ i ]->posColor.y );
		physics.circles[ i ]->setColor( c );
	}
}

void ColorCycle :: updateDelaunay ()
{
	delaunay.reset();
	
	for( int i=0; i<physics.circles.size(); i++ )
	{
		ofPoint& pos = physics.circles[ i ]->pos;
		delaunay.addPoint( pos.x, pos.y, pos.z );
	}
	
	delaunay.triangulate();
}

void ColorCycle :: updateTriangles ()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		delete triangles[ i ];
	}
	triangles.clear();
	
	//--
	
	for( int i=0; i<delaunay.triangles.size(); i++ )
	{
		const ofxDelaunayTriangle& delTri = delaunay.triangles[ i ];
		
		ColorTriangle* triangle;
		triangle = new ColorTriangle();
		
		for( int j=0; j<3; j++ )
		{
			//-- copy triangle points.
			
			triangle->points[ j ].x = delTri.points[ j ].x;
			triangle->points[ j ].y = delTri.points[ j ].y;
			
			//-- work out triangle colour.
			
			int c2 = ( j + 1 ) % 3;
			int c3 = ( j + 2 ) % 3;
			
			ofxVec2f p1, p2, p3;
			p1.set( delTri.points[ ( j + 0 ) % 3 ] );
			p2.set( delTri.points[ ( j + 1 ) % 3 ] );
			p3.set( delTri.points[ ( j + 2 ) % 3 ] );
			
			p2 -= p1;
			p2.normalize();
			
			p3 -= p1;
			p3.normalize();
			
			p2 += p3;
			p2.normalize();
			
			p1 += p2 * ( screen.screenMin * 0.25 );
			
			float px, py;
			px = p1.x / (float)screen.screenWidth;
			py = p1.y / (float)screen.screenHeight;
			
			px = MIN( MAX( px, 0.0 ), 1.0 );
			py = MIN( MAX( py, 0.0 ), 1.0 );
			
			ofColor c;
			c = interpolateColor( upperColor, lowerColor, py );
			
			triangle->colors[ j ] = c;
		}
		
		//--

		triangle->init();
		triangles.push_back( triangle );
	}
}

ofColor ColorCycle :: interpolateColor ( const ofColor& c1, const ofColor& c2, float p )
{
	ofColor c;
	c.r = ( c2.r - c1.r ) * p + c1.r;
	c.g = ( c2.g - c1.g ) * p + c1.g;
	c.b = ( c2.b - c1.b ) * p + c1.b;
	
	return c;
}

///////////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////////

void ColorCycle :: draw ()
{
	rect.draw();
	
	drawTriangles();
	
	physics.draw();
	
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

void ColorCycle :: drawTriangles ()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		triangles[ i ]->drawFill();
	}
	
	for( int i=0; i<triangles.size(); i++ )
	{
		triangles[ i ]->drawStroke();
	}
}

///////////////////////////////////////////////////////
//	PANEL.
///////////////////////////////////////////////////////

void ColorCycle :: showPanel ()
{
	bPanel = true;
	
	colorPicker0.show();
	colorPicker1.show();
}

void ColorCycle ::  hidePanel ()
{
	bPanel = false;
	
	colorPicker0.hide();
	colorPicker1.hide();
}

void ColorCycle ::  togglePanel	()
{
	bPanel = !bPanel;
	
	if( bPanel )
	{
		showPanel();
	}
	else
	{
		hidePanel();
	}
}

///////////////////////////////////////////////////////
//	JOINTS.
///////////////////////////////////////////////////////

void ColorCycle :: resetJoints ()
{
	physics.resetJoints();
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
