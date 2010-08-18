/*
 *  ColorPicker.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxColorPicker.h"

ofxColorPicker :: ofxColorPicker()
{
	init();
}

ofxColorPicker :: ~ofxColorPicker()
{
	//
}

//////////////////////////////////////////////
//	INIT.
//////////////////////////////////////////////

void ofxColorPicker :: init()
{
	dimensions.x		= 0;
	dimensions.y		= 0;
	dimensions.width	= 0;
	dimensions.height	= 0;
	
	colorScale			= 1.0;
	colorRadius			= 0;
	colorAngle			= 0;
	
	mousePoint			= getPoint( colorAngle, colorRadius );

	wheelLayer.radius		= 0;
	wheelLayer.colorScale	= colorScale;
	
	setSize( 0, 0, COLOR_PICKER_DEFAULT_WIDTH, COLOR_PICKER_DEFAULT_HEIGHT );

	colorWheel.enableMouseEvents();
	colorScaleBar.enableMouseEvents();

	show();
	enable();
	
	ofAddListener( ofEvents.update, this, &ofxColorPicker :: update );
	ofAddListener( ofEvents.exit,   this, &ofxColorPicker :: exit );
}

void ofxColorPicker :: exit( ofEventArgs &e )
{
	//
}

//////////////////////////////////////////////
//	RENDER RELATED METHODS.
//////////////////////////////////////////////

void ofxColorPicker :: setPos( float x, float y )
{
	checkDimensions( x, y, dimensions.width, dimensions.height );
}

void ofxColorPicker :: setSize( float x, float y, float w, float h )
{
	checkDimensions( x, y, w, h );
}

bool ofxColorPicker :: checkDimensions( int x, int y, int w, int h )
{
	if( dimensions.x != x || dimensions.y != y || dimensions.width != w || dimensions.height != h )
	{
		dimensions.x				= x;
		dimensions.y				= y;
		dimensions.width			= w;
		dimensions.height			= h;
		
		int minWH					= MIN( w, h );
		
		colorWheelRadius			= (int)( minWH * 0.5 * 0.9 );
		colorWheelPad				= (int)( ( minWH - colorWheelRadius * 2 ) * 0.5 );
		
		rectBackground.x			= (int)( dimensions.x );
		rectBackground.y			= (int)( dimensions.y );
		rectBackground.width		= (int)( dimensions.width );
		rectBackground.height		= (int)( dimensions.height );
		
		rectColorWheel.x			= (int)( dimensions.x + colorWheelPad );
		rectColorWheel.y			= (int)( dimensions.y + colorWheelPad );
		rectColorWheel.width		= (int)( colorWheelRadius * 2 );
		rectColorWheel.height		= (int)( colorWheelRadius * 2 );

		rectColorScaleBar.width		= (int)( dimensions.width - colorWheelPad * 2 );
		rectColorScaleBar.height	= (int)( rectColorScaleBar.width * 0.2 );
		rectColorScaleBar.x			= (int)( dimensions.x + colorWheelPad );
		rectColorScaleBar.y			= (int)( dimensions.y + colorWheelPad * 2 + colorWheelRadius * 2 );

		rectColorBox.x				= (int)( dimensions.x + colorWheelPad );
		rectColorBox.y				= (int)( rectColorScaleBar.y + rectColorScaleBar.height + colorWheelPad );
		rectColorBox.width			= (int)( dimensions.width - colorWheelPad * 2 );
		rectColorBox.height			= (int)( dimensions.height - ( rectColorBox.y - dimensions.y ) - colorWheelPad );
		
		colorWheel.setPos		( rectColorWheel.x,     rectColorWheel.y			);
		colorWheel.setSize		( rectColorWheel.width, rectColorWheel.height		);
		
		colorScaleBar.setPos	( rectColorScaleBar.x,     rectColorScaleBar.y		);
		colorScaleBar.setSize	( rectColorScaleBar.width, rectColorScaleBar.height	);
		
		return true;
	}
	
	return false;
}

float ofxColorPicker :: getWidth() 
{
	return dimensions.width;
}

float ofxColorPicker :: getHeight()
{ 
	return dimensions.height; 
}

//////////////////////////////////////////////
//	EVENT HANDLERS.
//////////////////////////////////////////////

void ofxColorPicker :: update()		// HACK - so update can be called manually when it needs to be.
{
	ofEventArgs e;
	update( e );
}

void ofxColorPicker :: update( ofEventArgs &e )
{
	updateColor();
	updateColorScale();
	
	color = getCircularColor( colorAngle, colorRadius, colorScale );
}

void ofxColorPicker :: updateColor ()
{
	if( colorWheel.isMouseDown() )
	{
		mousePoint.x = colorWheel.getMouseX() - rectColorWheel.x;
		mousePoint.y = colorWheel.getMouseY() - rectColorWheel.y;
		
		PolCord pc;
		pc = getPolarCoordinate( mousePoint.x, mousePoint.y );
		
		colorAngle	= pc.angle;
		colorRadius	= pc.radius;
		colorRadius = MIN( 1, MAX( 0, colorRadius ) );				// constrain to the radius of color circle.
	}
}

void ofxColorPicker :: updateColorScale()
{
	if( colorScaleBar.isMouseDown() )
	{
		int relX = colorScaleBar.getMouseX() - rectColorScaleBar.x;
		
		colorScale = MIN( 1, MAX( 0, relX / (float)( colorWheelRadius * 2 ) ) );
	}
}

//////////////////////////////////////////////
//	COLOR PICKER SETUP.
//////////////////////////////////////////////

void ofxColorPicker :: setColorScale( float s )
{
	colorScale = s;
}

float ofxColorPicker :: getColorScale ()
{
	return colorScale;
}

//------

void ofxColorPicker :: setColorRadius ( float r )
{
	colorRadius = r;
}

float ofxColorPicker :: getColorRadius ()
{
	return colorRadius;
}

//------

void ofxColorPicker :: setColorAngle ( float a )
{
	colorAngle = a * 360;
}

float ofxColorPicker :: getColorAngle ()
{
	return colorAngle / 360.0;
}

//////////////////////////////////////////////
//	VISIBLE.
//////////////////////////////////////////////

void ofxColorPicker :: show ()
{
	bVisible = true;
	
	colorWheel.enabled		= bVisible && bEnabled;
	colorScaleBar.enabled	= bVisible && bEnabled;
}

void ofxColorPicker :: hide ()
{
	bVisible = false;
	
	colorWheel.enabled		= bVisible && bEnabled;
	colorScaleBar.enabled	= bVisible && bEnabled;
}

void ofxColorPicker :: toggleShow ()
{
	bVisible = !bVisible;
	
	colorWheel.enabled		= bVisible && bEnabled;
	colorScaleBar.enabled	= bVisible && bEnabled;
}

bool ofxColorPicker :: isShowing ()
{
	return bVisible;
}

//////////////////////////////////////////////
//	ENABLE / DISABLE.
//////////////////////////////////////////////

void ofxColorPicker :: enable ()
{
	bEnabled = true;
	
	colorWheel.enabled		= bVisible && bEnabled;
	colorScaleBar.enabled	= bVisible && bEnabled;
}

void ofxColorPicker :: disable ()
{
	bEnabled = false;
	
	colorWheel.enabled		= bVisible && bEnabled;
	colorScaleBar.enabled	= bVisible && bEnabled;
}

//////////////////////////////////////////////
//	DISPLAY.
//////////////////////////////////////////////

void ofxColorPicker :: draw( float x, float y, float w, float h ) 
{
	setSize( x, y, w, h );
	draw();
}

void ofxColorPicker :: draw( float x, float y )
{
	setPos( x, y );
	draw();
}

void ofxColorPicker :: draw()
{
	if( !bVisible )
		return;
	
	drawBackground();
	drawColorWheel();
	drawColorPoint();
	drawColorScaleBar();
	drawColorRect();
	
	drawDebug();
}

void ofxColorPicker :: drawBackground()
{
	int x = rectBackground.x;
	int y = rectBackground.y;
	int w = rectBackground.width;
	int h = rectBackground.height;
	
	int bx, by;
	
	bx = by = 0;
	ofFill();
	ofSetColor( 113, 113, 113 );
	ofRect( x + bx, y + by, w - bx * 2, h - by * 2 );
	
	bx = by = 1;
	ofSetColor( 232, 232, 232 );
	ofRect( x + bx, y + by, w - bx * 2, h - by * 2 );
}

void ofxColorPicker :: drawColorWheel()
{
	int x = rectColorWheel.x;
	int y = rectColorWheel.y;
	int w = rectColorWheel.width;
	int h = rectColorWheel.height;
	
	if( wheelLayer.radius != colorWheelRadius || wheelLayer.colorScale != colorScale )
	{
		wheelLayer.radius		= colorWheelRadius;
		wheelLayer.colorScale	= colorScale;
		
		int cx = x + colorWheelRadius;
		int cy = y + colorWheelRadius;
		
		for( int i=0; i<COLOR_WHEEL_RES+1; i++ )
		{
			int j = i % COLOR_WHEEL_RES;
			float p = j / (float)COLOR_WHEEL_RES;
			float a = p * TWO_PI;
			
			wheelLayer.points[ i * 4 + 0 ] = cx + cos( -a ) * wheelLayer.radius;
			wheelLayer.points[ i * 4 + 1 ] = cy + sin( -a ) * wheelLayer.radius;
			
			wheelLayer.points[ i * 4 + 2 ] = cx;
			wheelLayer.points[ i * 4 + 3 ] = cy;
			
			ofColor c;
			c = getCircularColor( a * RAD_TO_DEG, 1.0, wheelLayer.colorScale );
			
			wheelLayer.colors[ i * 8 + 0 ] = c.r / 255.0;
			wheelLayer.colors[ i * 8 + 1 ] = c.g / 255.0;
			wheelLayer.colors[ i * 8 + 2 ] = c.b / 255.0;
			wheelLayer.colors[ i * 8 + 3 ] = 1.0;
			
			wheelLayer.colors[ i * 8 + 4 ] = 1.0;
			wheelLayer.colors[ i * 8 + 5 ] = 1.0;
			wheelLayer.colors[ i * 8 + 6 ] = 1.0;
			wheelLayer.colors[ i * 8 + 7 ] = 1.0;
		}
	}
	
	glEnable( GL_SMOOTH );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glVertexPointer( 2, GL_FLOAT, 0, &wheelLayer.points[ 0 ] );
	glColorPointer(  4, GL_FLOAT, 0, &wheelLayer.colors[ 0 ] );
	
	glDrawArrays( GL_TRIANGLE_STRIP, 0, ( COLOR_WHEEL_RES + 1 ) * 2 );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
}

void ofxColorPicker :: drawColorPoint()
{
	int x = rectColorWheel.x;
	int y = rectColorWheel.y;
	int w = rectColorWheel.width;
	int h = rectColorWheel.height;
	
	colorPoint = getPoint( colorAngle, colorRadius );
	colorPoint.x += x;
	colorPoint.y += y;
	
	colorPoint.x = ofClamp( colorPoint.x, x, x + w );
	colorPoint.y = ofClamp( colorPoint.y, y, y + h );
	
	ofFill();
	ofSetColor( 0, 0, 0 );
	ofCircle( colorPoint.x, colorPoint.y, 4 );
	ofSetColor( 255, 255, 255 );
	ofCircle( colorPoint.x, colorPoint.y, 2 );
	
	ofNoFill();
	ofSetColor( 0, 0, 0 );
	ofCircle( colorPoint.x, colorPoint.y, 4 );
	ofSetColor( 255, 255, 255 );
	ofCircle( colorPoint.x, colorPoint.y, 2 );
}

void ofxColorPicker :: drawColorScaleBar()
{
	int x = rectColorScaleBar.x;
	int y = rectColorScaleBar.y;
	int w = rectColorScaleBar.width;
	int h = rectColorScaleBar.height;
	
	ofFill();
	
	ofSetColor( 149, 149, 149 );
	ofRect( x, y, w, h );
	
	ofSetColor( 255, 255, 255 );
	ofRect( x + 1, y + 1, w - 2, h - 2 );
	
	ofColor c;
	c = getCircularColor( colorAngle, colorRadius, 1.0 );
	
	//--
	
	int rx, ry, rw, rh;
	rx = x + 2;
	ry = y + 2;
	rw = w - 4;
	rh = h - 4;
	
	rectScaleLayer.points[ 0 ] = rx;
	rectScaleLayer.points[ 1 ] = ry;
	rectScaleLayer.points[ 2 ] = rx;
	rectScaleLayer.points[ 3 ] = ry + rh;
	rectScaleLayer.points[ 4 ] = rx + rw;
	rectScaleLayer.points[ 5 ] = ry + rh;
	rectScaleLayer.points[ 6 ] = rx + rw;
	rectScaleLayer.points[ 7 ] = ry;
		
	rectScaleLayer.colors[ 0 ]  = 0;
	rectScaleLayer.colors[ 1 ]  = 0;
	rectScaleLayer.colors[ 2 ]  = 0;
	rectScaleLayer.colors[ 3 ]  = 1;
	rectScaleLayer.colors[ 4 ]  = 0;
	rectScaleLayer.colors[ 5 ]  = 0;
	rectScaleLayer.colors[ 6 ]  = 0;
	rectScaleLayer.colors[ 7 ]  = 1;
	rectScaleLayer.colors[ 8 ]  = c.r / 255.0;
	rectScaleLayer.colors[ 9 ]  = c.g / 255.0;
	rectScaleLayer.colors[ 10 ] = c.b / 255.0;
	rectScaleLayer.colors[ 11 ] = 1;
	rectScaleLayer.colors[ 12 ] = c.r / 255.0;
	rectScaleLayer.colors[ 13 ] = c.g / 255.0;
	rectScaleLayer.colors[ 14 ] = c.b / 255.0;
	rectScaleLayer.colors[ 15 ] = 1;
	
	glEnable( GL_SMOOTH );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glVertexPointer( 2, GL_FLOAT, 0, &rectScaleLayer.points[ 0 ] );
	glColorPointer(  4, GL_FLOAT, 0, &rectScaleLayer.colors[ 0 ] );
	
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	
	//--
	
	int bx, by;
	int cw = 3;
	int cx = colorScale * ( rw - cw );
	
	//-- red guide. this is not seen but is a guide for dev.
	
	ofSetColor( 255, 0, 0 );
	ofRect( rx + cx, ry - by, cw, rh + by * 2 );

	//-- grey rect for handle. bx and by are border values.
	
	bx = 2;
	by = 4;
	
	ofSetColor( 149, 149, 149 );
	ofRect( rx + cx - bx, ry - by, cw + bx * 2, rh + by * 2 );

	//-- white rect for handle. bx and by are border values.
	
	bx = 1;
	by = 3;
	
	ofSetColor( 255, 255, 255 );
	ofRect( rx + cx - bx, ry - by, cw + bx * 2, rh + by * 2 );
}

void ofxColorPicker :: drawColorRect()
{
	ofFill();

	int x = rectColorBox.x;
	int y = rectColorBox.y;
	int w = rectColorBox.width;
	int h = rectColorBox.height;
	
	int bx, by;

	bx = by = 0;
	ofSetColor( 149, 149, 149 );
	ofRect( x + bx, y + by, w - bx * 2, h - by * 2 );

	bx = by = 1;
	ofSetColor( 255, 255, 255 );
	ofRect( x + bx, y + by, w - bx * 2, h - by * 2 );
	
	bx = by = 2;
	ofSetColor( color.r, color.g, color.b, color.a );
	ofRect( x + bx, y + by, w - bx * 2, h - by * 2 );
}

void ofxColorPicker :: drawDebug ()
{
	
}

//////////////////////////////////////////////
//	COLOR MAPPING.
//////////////////////////////////////////////

void ofxColorPicker :: setColor( const ofColor& c )
{
	float da;
	
	if( ( c.r == c.g ) && ( c.r == c.b ) )
	{
		colorScale	= c.r / 255.0;
		colorRadius	= 0;
		colorAngle	= 0;
	}
	
	if( c.r > c.g && c.r > c.b )	// red.
	{
		colorScale = c.r / (float)255.0;
		
		if( c.g > c.b )	// between 0-60
		{
			colorRadius = 1 - c.b / (float)( 255.0 * colorScale );
			da = ( c.g / (float)( 255 * colorScale ) - ( 1 - colorRadius ) ) / colorRadius;
			colorAngle = da * 60; 
		}
		else				// between 300-360
		{
			colorRadius = 1 - c.g / (float)( 255.0 * colorScale );
			da = ( c.b / (float)( 255 * colorScale ) - ( 1 - colorRadius ) ) / colorRadius;
			colorAngle = 360 - da * 60;
		}
	}

	if( c.g > c.r && c.g > c.b )	// green.
	{
		colorScale = c.g / (float)255.0;
				  
		if( c.r > c.b )	// between 60-120
		{
			colorRadius = 1 - c.b / (float)( 255.0 * colorScale );
			da = ( c.r / (float)( 255 * colorScale ) - ( 1 - colorRadius ) ) / colorRadius;
			colorAngle = 120 - da * 60;
		}
		else				// between 120-180
		{
			colorRadius = 1 - c.r / (float)( 255.0 * colorScale );
			da = ( c.b / (float)( 255 * colorScale ) - ( 1 - colorRadius ) ) / colorRadius;
			colorAngle = da * 60 + 120;
		}
	}
				  
	if( c.b > c.r && c.b > c.g )	// blue.
	{
		colorScale = c.b / (float)255.0;
		
		if( c.g > c.r )	// between 180-240
		{
			colorRadius = 1 - c.r / (float)( 255.0 * colorScale );
			da = ( c.g / (float)( 255 * colorScale ) - ( 1 - colorRadius ) ) / colorRadius;
			colorAngle = 240 - da * 60;
		}
		else				// between 240-300
		{
			colorRadius = 1 - c.g / (float)( 255.0 * colorScale );
			da = ( c.r / (float)( 255 * colorScale ) - ( 1 - colorRadius ) ) / colorRadius;
			colorAngle = da * 60 + 240;
		}
	}
}

ofColor ofxColorPicker :: getColor()
{
	ofColor c;
	c.r = color.r;
	c.g = color.g;
	c.b = color.b;
	c.a = color.a;
	
	return c;
}

ofColor ofxColorPicker :: getCircularColor( float angle, float radius, float scale )
{
	radius = MIN( 1.0, MAX( 0.0, radius ) );
	
	angle -= floor( angle / 360 ) * 360;
	if( angle < 0 )
		angle += 360;
	
	float da;
	
	ofColor c;
	
	if( angle < 60 )
	{
		da  = angle / 60;
		c.r = (int)( 255 * scale );
		c.g = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c.b = (int)( 255 * ( 1 - radius ) * scale );
		c.a = 255;
	}
	else if( angle < 120 )
	{
		da  = ( 120 - angle ) / 60;
		c.r = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c.g = (int)( 255 * scale );
		c.b = (int)( 255 * ( 1 - radius ) * scale );
		c.a = 255;
	}
	else if( angle < 180 )
	{
		da  = 1 - ( 180 - angle ) / 60;
		c.r = (int)( 255 * ( 1 - radius ) * scale );
		c.g = (int)( 255 * scale );
		c.b = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c.a = 255;
	}
	else if( angle < 240 )
	{
		da  = ( 240 - angle ) / 60;
		c.r = (int)( 255 * ( 1 - radius ) * scale );
		c.g = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c.b = (int)( 255 * scale );
		c.a = 255;
	}
	else if( angle < 300 )
	{
		da  = 1 - ( 300 - angle ) / 60;
		c.r = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c.g = (int)( 255 * ( 1 - radius ) * scale );
		c.b = (int)( 255 * scale );
		c.a = 255;
	}
	else if( angle <= 360 )
	{
		da  = ( 360 - angle ) / 60;
		c.r = (int)( 255 * scale );
		c.g = (int)( 255 * ( 1 - radius ) * scale );
		c.b = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c.a = 255;
	}
	
	return c;
}

ofPoint ofxColorPicker :: getPoint ( float a, float r )
{
	float cx = colorWheelRadius;
	float cy = colorWheelRadius;
	
	ofPoint p;
	p.x = cx * r * cos( -a * DEG_TO_RAD ) + cx;
	p.y = cy * r * sin( -a * DEG_TO_RAD ) + cy;
	
	return p;
}

PolCord ofxColorPicker :: getPolarCoordinate ( float x, float y )
{
	float cx = colorWheelRadius;
	float cy = colorWheelRadius;
	
	float px = x - cx;						// point x from center.
	float py = y - cy;						// point x from center.
	float pl = sqrt( px * px + py * py );	// point length from center.
	float pa = atan2( px, py );				// point angle around center.
	
	pa *= RAD_TO_DEG;
	pa -= 90;
	pl /= cx;
	
	PolCord pc;
	pc.angle	= pa;
	pc.radius	= pl;
	
	return pc;
}
