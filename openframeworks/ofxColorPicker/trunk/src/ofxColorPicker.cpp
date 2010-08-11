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
//	RENDER RELATED METHODS.
//////////////////////////////////////////////

void ofxColorPicker :: draw()
{
	if( !visible )
		return;
	
	glPushMatrix();
	glTranslatef( dimensions.x, dimensions.y, 0 );
	
	drawBackground( colorWheelWidth, colorWheelHeight );
	drawColorWheel( colorWheelWidth * 0.5 );
	drawColorPoint( colorWheelWidth, colorWheelHeight );
	drawColorScaleBar( colorWheelWidth, colorWheelHeight );
	drawColorRect( colorWheelWidth, colorWheelHeight );
	
	glPopMatrix();
}

void ofxColorPicker :: draw( float x, float y )
{
	setPos( x, y );
	draw();
}

void ofxColorPicker :: draw( float x, float y, float w, float h ) 
{
	setSize( x, y, w, h );
	draw();
}

void ofxColorPicker :: setPos( float x, float y )
{
	if( checkPos( x, y ) )
	{
		colorWheel.setPos( dimensions.x, dimensions.y );
		colorScaleBar.setPos( dimensions.x, dimensions.y + colorWheelHeight + 20 );
	}
}

void ofxColorPicker :: setSize( float x, float y, float w, float h )
{
	if( checkPos( x, y ) || checkSize( w, h ) )
	{
		colorWheel.setPos( dimensions.x, dimensions.y );
		colorWheel.setSize( colorWheelWidth, colorWheelHeight );
		
		colorScaleBar.setPos( dimensions.x, dimensions.y + colorWheelHeight + 20 );
		colorScaleBar.setSize( colorWheelWidth, 20 );
	}
}

bool ofxColorPicker :: checkPos( int x, int y )
{
	bool posChange = false;
	
	if( dimensions.x != x )
	{
		dimensions.x = x;
		posChange	 = true;
	}
	
	if( dimensions.y != y )
	{
		dimensions.y = y;
		posChange	 = true;
	}
	
	return posChange;
}

bool ofxColorPicker :: checkSize( int w, int h )
{
	bool sizeChange = false;

	if( colorWheelWidth	!= w )
	{
		colorWheelWidth  = w;
		sizeChange		 = true;
	}
	
	if( colorWheelHeight != h )
	{
		colorWheelHeight = h;
		sizeChange		 = true;
	}
	
	return sizeChange;
}

float ofxColorPicker :: getWidth() 
{
	return colorWheelWidth + 30;
}

float ofxColorPicker :: getHeight()
{ 
	return colorWheelHeight + 190; 
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
	if( colorMode == COLOR_PICKER_MODE_CIRLCE_ROTATION )
		updateCircularInterpolation();
	
	if( colorMode == COLOR_PICKER_MODE_RANDOM_WALK )
		updateRandomWalkInterpolation();
	
	if( colorMode == COLOR_PICKER_MODE_MOUSE )
		updateMouseInterpolation();
	
	updateColorScale();
	
	getCircularColor( colorAngle, colorRadius, colorScale, &color );
}

void ofxColorPicker :: exit( ofEventArgs &e )
{
	//
}

//////////////////////////////////////////////
//	COLOR PICKER SETUP.
//////////////////////////////////////////////

void ofxColorPicker :: init()
{
	dimensions.x	 = 0;
	dimensions.y	 = 0;
	colorWheelWidth  = COLOR_PICKER_DEFAULT_WIDTH;
	colorWheelHeight = COLOR_PICKER_DEFAULT_HEIGHT;
	
	colorScale			= 1.0;
	colorRadius			= 0;
	colorAngle			= 0;
	
	oscillationAngle	= 0;
	
	wheelLayer.radius		= COLOR_WHEEL_RADIUS;
	wheelLayer.colorScale	= colorScale;
	
	show();
	
	setMode( COLOR_PICKER_MODE_MOUSE );
	
	setCircularSpeed( 1.0 );
	setCircularOscillationSpeed( 0.1 );
	setCircularUpperBounds( 1.0 );
	setCircularLowerBounds( 0.0 );
	
	setRandomWalkVelocityMax( 4.0 );
	setRandomWalkVelocityEase( 0.1 );
	
	colorWheel.setPos( dimensions.x, dimensions.y );
	colorWheel.setSize( colorWheelWidth, colorWheelHeight );
	colorWheel.enableMouseEvents();
	
	colorScaleBar.setPos( dimensions.x, dimensions.y + colorWheelHeight + 20 );
	colorScaleBar.setSize( colorWheelWidth, 20 );
	colorScaleBar.enableMouseEvents();
	
	ofAddListener( ofEvents.update, this, &ofxColorPicker :: update );
	ofAddListener( ofEvents.exit,   this, &ofxColorPicker :: exit );
}

void ofxColorPicker :: setMode( int m )
{
	if
	(
		m == COLOR_PICKER_MODE_CIRLCE_ROTATION	||
		m == COLOR_PICKER_MODE_RANDOM_WALK		||
		m == COLOR_PICKER_MODE_MOUSE
	)
	{
		colorMode = m;
	}
	
	if( m == COLOR_PICKER_MODE_CIRLCE_ROTATION )
		initCircularInterpolation();
	
	if( m == COLOR_PICKER_MODE_RANDOM_WALK )
		initRandomWalkInterpolation();
	
	if( m == COLOR_PICKER_MODE_MOUSE )
		initMouseInterpolation();
}

void ofxColorPicker :: setRandomColor()
{
	colorAngle  = ofRandom( 0, 360 );
	colorRadius = ofRandom( 0, 1 );
	colorScale  = ofRandom( 0, 1 );
	
	getCircularColor( colorAngle, colorRadius, colorScale, &color );
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

void ofxColorPicker :: setColorScale( float s )
{
	colorScale = s;
}

void ofxColorPicker :: setColorRadius ( float r )
{
	colorRadius = r;
}

void ofxColorPicker :: setColorAngle ( float a )
{
	colorAngle = a * 360;
}

void ofxColorPicker :: addToColorAngle ( float a )
{
	colorAngle += ( a * 360 );
}

//////////////////////////////////////////////
//	VISIBLE.
//////////////////////////////////////////////

void ofxColorPicker :: show ()
{
	visible = true;
}

void ofxColorPicker :: hide ()
{
	visible = false;
}

void ofxColorPicker :: toggleShow ()
{
	visible = !visible;
}

bool ofxColorPicker :: isShowing ()
{
	return visible;
}

//////////////////////////////////////////////
//	COLOR SCALE.
//////////////////////////////////////////////

void ofxColorPicker :: updateColorScale()
{
	if( colorScaleBar.isMouseDown() )
	{
		int relX = colorScaleBar.getMouseX() - dimensions.x;
		
		colorScale = MIN( 1, MAX( 0, relX / (float)colorWheelWidth ) );
	}
}

//////////////////////////////////////////////
//	CIRCULAR INTERPOLATION.
//////////////////////////////////////////////

void ofxColorPicker :: initCircularInterpolation()
{
	//
}

void ofxColorPicker :: updateCircularInterpolation()
{
	colorAngle += colorAngleInc;
	
	oscillationAngle += oscillationAngleInc;
	colorRadius = ( upperColorRadiusBounds - lowerColorRadiusBounds ) * ( 0.5 * sin( oscillationAngle * DEG_TO_RAD ) + 0.5 ) + lowerColorRadiusBounds;
}

void ofxColorPicker :: setCircularUpperBounds( float value )
{
	upperColorRadiusBounds = MIN( 1, MAX( 0, value ) );
}

void ofxColorPicker :: setCircularLowerBounds( float value )
{
	lowerColorRadiusBounds = MIN( 1, MAX( 0, value ) );
}

void ofxColorPicker :: setCircularSpeed( float value )
{
	colorAngleInc = value;
}

void ofxColorPicker :: setCircularOscillationSpeed( float value )
{
	oscillationAngleInc = value;
}

//////////////////////////////////////////////
//	RANDOM WALK INTERPOLATION.
//////////////////////////////////////////////

void ofxColorPicker :: initRandomWalkInterpolation()
{
	getPoint( colorAngle, colorRadius, &randomWalkPoint );
}

void ofxColorPicker :: updateRandomWalkInterpolation()
{
	randomWalkVel.x += ( ( ofRandom( -randomWalkVelMax, randomWalkVelMax ) ) - randomWalkVel.x ) * randomWalkVelEase;
	randomWalkVel.y += ( ( ofRandom( -randomWalkVelMax, randomWalkVelMax ) ) - randomWalkVel.y ) * randomWalkVelEase;
	
	randomWalkPoint.x += randomWalkVel.x;
	randomWalkPoint.y += randomWalkVel.y;

	getAngleCoordinates( randomWalkPoint.x, randomWalkPoint.y, &colorAngle, &colorRadius );
	
	colorRadius = MIN( 1, MAX( 0, colorRadius ) );				// constrain to the radius of color circle.
	
	getPoint( colorAngle, colorRadius, &randomWalkPoint );		// re-calc contstrained values back to XY.
}

void ofxColorPicker :: setRandomWalkVelocityMax( float value )
{
	randomWalkVelMax = value;
}

void ofxColorPicker :: setRandomWalkVelocityEase( float value )
{
	randomWalkVelEase = value;
}

//////////////////////////////////////////////
//	MOUSE INTERPOLATION.
//////////////////////////////////////////////

void ofxColorPicker :: initMouseInterpolation()
{
	getPoint( colorAngle, colorRadius, &mousePoint );
}

void ofxColorPicker :: updateMouseInterpolation()
{
	if( colorWheel.isMouseDown() )
	{
		mousePoint.x = colorWheel.getMouseX() - dimensions.x;
		mousePoint.y = colorWheel.getMouseY() - dimensions.y;
		
		getAngleCoordinates( mousePoint.x, mousePoint.y, &colorAngle, &colorRadius );
		
		colorRadius = MIN( 1, MAX( 0, colorRadius ) );
	}
}

//////////////////////////////////////////////
//	DISPLAY.
//////////////////////////////////////////////

void ofxColorPicker :: drawBackground( int w, int h )
{
	int bx, by;
	
	bx = by = 10;
	ofFill();
	ofSetColor( 113, 113, 113 );
	ofRect( -bx, -by, w + bx * 2, h + 160 + bx * 2 );
	
	bx = by = 9;
	ofSetColor( 232, 232, 232 );
	ofRect( -bx, -by, w + bx * 2, h + 160 + bx * 2 );	
}

void ofxColorPicker :: drawColorWheel( float radius )
{
	if( wheelLayer.radius !=radius || wheelLayer.colorScale != colorScale )
	{
		wheelLayer.radius		= radius;
		wheelLayer.colorScale	= colorScale;
		
		for( int i=0; i<COLOR_WHEEL_RES+1; i++ )
		{
			int j = i % COLOR_WHEEL_RES;
			float p = j / (float)COLOR_WHEEL_RES;
			float a = p * TWO_PI;
			
			wheelLayer.points[ i * 4 + 0 ] = cos( -a ) * wheelLayer.radius;
			wheelLayer.points[ i * 4 + 1 ] = sin( -a ) * wheelLayer.radius;
			
			wheelLayer.points[ i * 4 + 2 ] = 0;
			wheelLayer.points[ i * 4 + 3 ] = 0;
			
			ofColor c;
			getCircularColor( a * RAD_TO_DEG, 1.0, wheelLayer.colorScale, &c );
			
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
	
	glPushMatrix();
	glTranslatef( radius, radius, 0 );
	
	glEnable( GL_SMOOTH );
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	
	glVertexPointer( 2, GL_FLOAT, 0, &wheelLayer.points[ 0 ] );
	glColorPointer(  4, GL_FLOAT, 0, &wheelLayer.colors[ 0 ] );
	
	glDrawArrays( GL_TRIANGLE_STRIP, 0, ( COLOR_WHEEL_RES + 1 ) * 2 );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	
	glPopMatrix();
}

void ofxColorPicker :: drawColorPoint( int w, int h )
{
	getPoint( colorAngle, colorRadius, &colorPoint );
	
	if( colorPoint.x < 0 || colorPoint.x > w )
		return;
	
	if( colorPoint.y < 0 || colorPoint.y > h )
		return;
	
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

void ofxColorPicker :: drawColorScaleBar( int w, int h )
{
	ofFill();
	
	int bx, by;
	
	bx = by = 2;
	ofSetColor( 149, 149, 149 );
	ofRect( -bx, h + 20 - bx, w + bx * 2, 20 + bx * 2 );
	
	bx = by = 1;
	ofSetColor( 255, 255, 255 );
	ofRect( -bx, h + 20 - bx, w + bx * 2, 20 + bx * 2 );
	
	bx = by = 0;
	
	ofColor c;
	getCircularColor( colorAngle, colorRadius, 1.0, &c );
	
	//--
	
	int rx, ry, rw, rh;
	rx = -bx;
	ry = h + 20 - bx;
	rw = w + bx * 2;
	rh = 20 + bx * 2;
	
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
	
	int cx = colorScale * w;
	int cw = 3;
	
	bx = 2;
	by = 4;
	ofSetColor( 149, 149, 149 );
	ofFill();
	ofRect( cx - bx, h + 20 - by, cw + bx * 2, 20 + by * 2 );

	bx = 1;
	by = 3;
	ofSetColor( 255, 255, 255 );
	ofFill();
	ofRect( cx - bx, w + 20 - by, cw + bx * 2, 20 + by * 2 );
}

void ofxColorPicker :: drawColorRect( int w, int h )
{
	ofFill();
	
	int bx, by;

	bx = by = 2;
	ofSetColor( 149, 149, 149 );
	ofRect( -bx, h + 60 - bx, w + bx * 2, 100 + bx * 2 );

	bx = by = 1;
	ofSetColor( 255, 255, 255 );
	ofRect( -bx, h + 60 - bx, w + bx * 2, 100 + bx * 2 );
	
	bx = by = 0;
	ofSetColor( color.r, color.g, color.b, color.a );
	ofRect( -bx, h + 60 - bx, w + bx * 2, 100 + bx * 2 );
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

void ofxColorPicker :: getCircularColor( float angle, float radius, float scale, ofColor *c )
{
	radius = MIN( 1.0, MAX( 0.0, radius ) );
	
	angle -= floor( angle / 360 ) * 360;
	if( angle < 0 )
		angle += 360;
	
	float da;
	
	if( angle < 60 )
	{
		da   = angle / 60;
		c->r = (int)( 255 * scale );
		c->g = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c->b = (int)( 255 * ( 1 - radius ) * scale );
		c->a = 255;
	}
	else if( angle < 120 )
	{
		da   = ( 120 - angle ) / 60;
		c->r = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c->g = (int)( 255 * scale );
		c->b = (int)( 255 * ( 1 - radius ) * scale );
		c->a = 255;
	}
	else if( angle < 180 )
	{
		da   = 1 - ( 180 - angle ) / 60;
		c->r = (int)( 255 * ( 1 - radius ) * scale );
		c->g = (int)( 255 * scale );
		c->b = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c->a = 255;
	}
	else if( angle < 240 )
	{
		da   = ( 240 - angle ) / 60;
		c->r = (int)( 255 * ( 1 - radius ) * scale );
		c->g = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c->b = (int)( 255 * scale );
		c->a = 255;
	}
	else if( angle < 300 )
	{
		da   = 1 - ( 300 - angle ) / 60;
		c->r = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c->g = (int)( 255 * ( 1 - radius ) * scale );
		c->b = (int)( 255 * scale );
		c->a = 255;
	}
	else if( angle <= 360 )
	{
		da   = ( 360 - angle ) / 60;
		c->r = (int)( 255 * scale );
		c->g = (int)( 255 * ( 1 - radius ) * scale );
		c->b = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * scale );
		c->a = 255;
	}
}

void ofxColorPicker :: getPoint( float a, float r, ofPoint *p )
{
	float cx = colorWheelWidth  * 0.5;
	float cy = colorWheelHeight * 0.5;
	
	p->x = cx * r * cos( -a * DEG_TO_RAD ) + cx;
	p->y = cy * r * sin( -a * DEG_TO_RAD ) + cy;
}

void ofxColorPicker :: getAngleCoordinates( float x, float y, float *a, float *r )
{
	float cx = colorWheelWidth * 0.5;
	float cy = colorWheelHeight * 0.5;
	
	float px = x - cx;						// point x from center.
	float py = y - cy;						// point x from center.
	float pl = sqrt( px * px + py * py );	// point length from center.
	float pa = atan2( px, py );				// point angle around center.
	
	pa *= RAD_TO_DEG;
	pa -= 90;
	pl /= cx;
	
	*a = pa;
	*r = pl;
}
