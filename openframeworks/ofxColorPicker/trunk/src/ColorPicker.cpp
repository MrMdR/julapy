/*
 *  ColorPicker.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ColorPicker.h"

ColorPicker :: ColorPicker()
{

}

ColorPicker :: ~ColorPicker()
{

}

//////////////////////////////////////////////
//	PUBLIC.
//////////////////////////////////////////////

void ColorPicker :: init( int x, int y, int width, int height )
{
	dimensions.x	 = x;
	dimensions.y	 = y;
	colorWheelWidth  = width;
	colorWheelHeight = height;

	colorScale			= 1.0;
	colorRadius			= 0;
	colorAngle			= 0;
	
	oscillationAngle	= 0;
	
	setVisible( true );
	
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
	
	ofAddListener( ofEvents.update, this, &ColorPicker :: update );
	ofAddListener( ofEvents.draw,   this, &ColorPicker :: draw );
	ofAddListener( ofEvents.exit,   this, &ColorPicker :: exit );
}

void ColorPicker :: update( ofEventArgs &e )
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

void ColorPicker :: draw( ofEventArgs &e )
{
	if( !visible )
		return;
	
	glPushMatrix();
	glTranslatef( dimensions.x, dimensions.y, 0 );
	
	drawBackground();
	drawColorWheel();
	drawColorPoint();
	drawColorScaleBar();
	drawColorRect();
	
	glPopMatrix();
}

void ColorPicker :: exit( ofEventArgs &e )
{
	//
}

void ColorPicker :: setMode( int m )
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

void ColorPicker :: setColorScale( float s )
{
	colorScale = s;
}

void ColorPicker :: setRandomColor()
{
	colorAngle  = ofRandom( 0, 360 );
	colorRadius = ofRandom( 0, 1 );
	colorScale  = ofRandom( 0, 1 );
}

void ColorPicker :: setColor( Color *c )
{
	//
}

void ColorPicker :: getColor( Color *c )
{
	c->r = color.r;
	c->g = color.g;
	c->b = color.b;
	c->a = color.a;
}

//////////////////////////////////////////////
//	VISIBLE.
//////////////////////////////////////////////

void ColorPicker :: setVisible( bool b )
{
	visible = b;
}

bool ColorPicker :: isVisible()
{
	return visible;
}

//////////////////////////////////////////////
//	COLOR SCALE.
//////////////////////////////////////////////

void ColorPicker :: updateColorScale()
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

void ColorPicker :: initCircularInterpolation()
{
	//
}

void ColorPicker :: updateCircularInterpolation()
{
	colorAngle += colorAngleInc;
	
	oscillationAngle += oscillationAngleInc;
	colorRadius = ( upperColorRadiusBounds - lowerColorRadiusBounds ) * ( 0.5 * sin( oscillationAngle * DEG_TO_RAD ) + 0.5 ) + lowerColorRadiusBounds;
}

void ColorPicker :: setCircularUpperBounds( float value )
{
	upperColorRadiusBounds = MIN( 1, MAX( 0, value ) );
}

void ColorPicker :: setCircularLowerBounds( float value )
{
	lowerColorRadiusBounds = MIN( 1, MAX( 0, value ) );
}

void ColorPicker :: setCircularSpeed( float value )
{
	colorAngleInc = value;
}

void ColorPicker :: setCircularOscillationSpeed( float value )
{
	oscillationAngleInc = value;
}

//////////////////////////////////////////////
//	RANDOM WALK INTERPOLATION.
//////////////////////////////////////////////

void ColorPicker :: initRandomWalkInterpolation()
{
	getPoint( colorAngle, colorRadius, &randomWalkPoint );
}

void ColorPicker :: updateRandomWalkInterpolation()
{
	randomWalkVel.x += ( ( ofRandom( -randomWalkVelMax, randomWalkVelMax ) ) - randomWalkVel.x ) * randomWalkVelEase;
	randomWalkVel.y += ( ( ofRandom( -randomWalkVelMax, randomWalkVelMax ) ) - randomWalkVel.y ) * randomWalkVelEase;
	
	randomWalkPoint.x += randomWalkVel.x;
	randomWalkPoint.y += randomWalkVel.y;

	getAngleCoordinates( randomWalkPoint.x, randomWalkPoint.y, &colorAngle, &colorRadius );
	
	colorRadius = MIN( 1, MAX( 0, colorRadius ) );				// constrain to the radius of color circle.
	
	getPoint( colorAngle, colorRadius, &randomWalkPoint );		// re-calc contstrained values back to XY.
}

void ColorPicker :: setRandomWalkVelocityMax( float value )
{
	randomWalkVelMax = value;
}

void ColorPicker :: setRandomWalkVelocityEase( float value )
{
	randomWalkVelEase = value;
}

//////////////////////////////////////////////
//	MOUSE INTERPOLATION.
//////////////////////////////////////////////

void ColorPicker :: initMouseInterpolation()
{
	getPoint( colorAngle, colorRadius, &mousePoint );
}

void ColorPicker :: updateMouseInterpolation()
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

void ColorPicker :: drawBackground()
{
	int bx, by;
	
	bx = by = 10;
	ofFill();
	ofSetColor( 113, 113, 113 );
	ofRect( -bx, -by, colorWheelWidth + bx * 2, colorWheelHeight + 160 + bx * 2 );
	
	bx = by = 9;
	ofSetColor( 232, 232, 232 );
	ofRect( -bx, -by, colorWheelWidth + bx * 2, colorWheelHeight + 160 + bx * 2 );	
}

void ColorPicker :: drawColorWheel()
{
	int res				= 200;
	float angle			= 0.0f;
	float angleAdder	= M_TWO_PI / (float)res;
	float radius		= colorWheelWidth * 0.5;
	int k				= 0;
	
	float *circlePts	= new float[ ( res + 1 ) * 2 ];
	float *circleAngs	= new float[ res + 1 ];
	
	for( int i=0; i<res + 1; i++ )
	{
		circleAngs[ i ]    = angle * RAD_TO_DEG;
		circlePts[ k + 0 ] = cos( -angle );
		circlePts[ k + 1 ] = sin( -angle );
		angle	+= angleAdder;
		k		+= 2;
	}
	
	glPushMatrix();
	glTranslatef( colorWheelWidth * 0.5, colorWheelHeight * 0.5, 0 );
	
	Color c;
	
	glEnable( GL_SMOOTH );
	
	for( int i=0; i<res; i++ )
	{
		glBegin( GL_TRIANGLES );
		
			getCircularColor( circleAngs[ i + 0 ], 0.0, colorScale, &c );
			glColor3f( c.r / 255.0, c.g / 255.0, c.b / 255.0 );
			glVertex2f( 0, 0 );

			getCircularColor( circleAngs[ i + 0 ], 1.0, colorScale, &c );
			glColor3f( c.r / 255.0, c.g / 255.0, c.b / 255.0 );
			glVertex2f( circlePts[ i * 2 + 0 ] * radius, circlePts[ i * 2 + 1 ] * radius );

			getCircularColor( circleAngs[ i + 1 ], 1.0, colorScale, &c );
			glColor3f( c.r / 255.0, c.g / 255.0, c.b / 255.0 );
			glVertex2f( circlePts[ i * 2 + 2 ] * radius, circlePts[ i * 2 + 3 ] * radius );
		
		glEnd();
	}
	
	glPopMatrix();
	
	delete[] circlePts;
	delete[] circleAngs;
}

void ColorPicker :: drawColorPoint()
{
	getPoint( colorAngle, colorRadius, &colorPoint );
	
	if( colorPoint.x < 0 || colorPoint.x > colorWheelWidth )
		return;
	
	if( colorPoint.y < 0 || colorPoint.y > colorWheelHeight )
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

void ColorPicker :: drawColorScaleBar()
{
	ofFill();
	
	int bx, by;
	
	bx = by = 2;
	ofSetColor( 149, 149, 149 );
	ofRect( -bx, colorWheelHeight + 20 - bx, colorWheelWidth + bx * 2, 20 + bx * 2 );
	
	bx = by = 1;
	ofSetColor( 255, 255, 255 );
	ofRect( -bx, colorWheelHeight + 20 - bx, colorWheelWidth + bx * 2, 20 + bx * 2 );
	
	bx = by = 0;
	
	Color c;
	getCircularColor( colorAngle, colorRadius, 1.0, &c );
	
	int rx, ry, rw, rh;
	rx = -bx;
	ry = colorWheelHeight + 20 - bx;
	rw = colorWheelWidth + bx * 2;
	rh = 20 + bx * 2;
	
	glEnable( GL_SMOOTH );
	glBegin( GL_QUADS );
		glColor3f( 0, 0, 0 );
		glVertex2f( rx, ry );
		glVertex2f( rx, ry + rh );
		glColor3f( c.r / 255.0, c.g / 255.0, c.b / 255.0 );
		glVertex2f( rx + rw, ry + rh );
		glVertex2f( rx + rw, ry );
	glEnd();
	
	int cx = colorScale * colorWheelWidth;
	int cw = 3;
	
	bx = 2;
	by = 4;
	ofSetColor( 149, 149, 149 );
	ofFill();
	ofRect( cx - bx, colorWheelHeight + 20 - by, cw + bx * 2, 20 + by * 2 );

	bx = 1;
	by = 3;
	ofSetColor( 255, 255, 255 );
	ofFill();
	ofRect( cx - bx, colorWheelHeight + 20 - by, cw + bx * 2, 20 + by * 2 );
}

void ColorPicker :: drawColorRect()
{
	ofFill();
	
	int bx, by;

	bx = by = 2;
	ofSetColor( 149, 149, 149 );
	ofRect( -bx, colorWheelHeight + 60 - bx, colorWheelWidth + bx * 2, 100 + bx * 2 );

	bx = by = 1;
	ofSetColor( 255, 255, 255 );
	ofRect( -bx, colorWheelHeight + 60 - bx, colorWheelWidth + bx * 2, 100 + bx * 2 );
	
	bx = by = 0;
	ofSetColor( color.r, color.g, color.b, color.a );
	ofRect( -bx, colorWheelHeight + 60 - bx, colorWheelWidth + bx * 2, 100 + bx * 2 );
}

//////////////////////////////////////////////
//	COLOR MAPPING.
//////////////////////////////////////////////

void ColorPicker :: getCircularColor( float angle, float radius, float scale, Color *c )
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
		c->r = (int)( 255 * ( 1 - radius ) * colorScale );
		c->g = (int)( 255 * ( da + ( 1 - da ) * ( 1 - radius ) ) * colorScale );
		c->b = (int)( 255 * colorScale );
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

void ColorPicker :: getPoint( float a, float r, ofPoint *p )
{
	float cx = colorWheelWidth  * 0.5;
	float cy = colorWheelHeight * 0.5;
	
	p->x = cx * r * cos( -a * DEG_TO_RAD ) + cx;
	p->y = cy * r * sin( -a * DEG_TO_RAD ) + cy;
}

void ColorPicker :: getAngleCoordinates( float x, float y, float *a, float *r )
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
