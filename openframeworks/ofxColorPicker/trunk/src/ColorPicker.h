/*
 *  ColorPicker.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _COLOR_PICKER_H_
#define _COLOR_PICKER_H_

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

#define		COLOR_PICKER_MODE_CIRLCE_ROTATION		0
#define		COLOR_PICKER_MODE_RANDOM_WALK			1
#define		COLOR_PICKER_MODE_MOUSE					2

class Color
{
public :
	int r, g, b, a;
};

class ColorPicker
{

public :
	
	ColorPicker();
	~ColorPicker();
	
	void init( int x, int y, int width, int height );
	void update( ofEventArgs &e );
	void draw( ofEventArgs &e );
	void exit( ofEventArgs &e );
	
	void setVisible( bool b );
	bool isVisible();
	
	void setMode( int m );
	void setColorScale( float s );
	void setColor( Color *c );
	void getColor( Color *c );
	
	void setCircularUpperBounds( float value );
	void setCircularLowerBounds( float value );
	void setCircularSpeed( float value );
	void setCircularOscillationSpeed( float value );
	
	void setRandomWalkVelocityMax( float value );
	void setRandomWalkVelocityEase( float value );
	
private :

	void updateColorScale();
	
	void initCircularInterpolation();
	void updateCircularInterpolation();
	
	void initRandomWalkInterpolation();
	void updateRandomWalkInterpolation();
	
	void initMouseInterpolation();
	void updateMouseInterpolation();
	
	void drawBackground();
	void drawColorWheel();
	void drawColorPoint();
	void drawColorScaleBar();
	void drawColorRect();

	void getCircularColor( float angle, float radius, float scale, Color *c );
	void getPoint( float a, float r, ofPoint *p );
	void getAngleCoordinates( float x, float y, float *a, float *r );

	ofxMSAInteractiveObject colorWheel;
	ofxMSAInteractiveObject colorScaleBar;
	
	bool		visible;
	
	Color		color;
	ofPoint		colorPoint;
	float		colorScale;
	float		colorRadius;				
	float		colorAngle;
	int			colorMode;
	
	float		colorAngleInc;				// circular interpolation.
	float		upperColorRadiusBounds;
	float		lowerColorRadiusBounds;
	float		oscillationAngle;
	float		oscillationAngleInc;
	
	ofPoint		randomWalkPoint;			// radom walk interpolation.
	ofPoint		randomWalkVel;
	float		randomWalkVelMax;
	float		randomWalkVelEase;
	
	ofPoint		mousePoint;					// mouse interpolation.
	
	ofRectangle dimensions;
	int			colorWheelWidth;			// color wheel display.
	int			colorWheelHeight;
};

#endif