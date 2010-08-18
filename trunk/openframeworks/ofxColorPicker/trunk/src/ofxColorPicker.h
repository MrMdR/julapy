/*
 *  ColorPicker.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OFX_COLOR_PICKER_H
#define OFX_COLOR_PICKER_H

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

#define		COLOR_PICKER_DEFAULT_WIDTH				100
#define		COLOR_PICKER_DEFAULT_HEIGHT				200

#define		COLOR_WHEEL_RES							200

#define		COLOR_PICKER_MODE_CIRLCE_ROTATION		0
#define		COLOR_PICKER_MODE_RANDOM_WALK			1
#define		COLOR_PICKER_MODE_MOUSE					2

struct Rectangle
{
	float	points[ 2 * 4 ];
	float	colors[ 4 * 4 ];
};

struct ColorWheel
{
	float	points[ ( COLOR_WHEEL_RES + 1 ) * 2 * 2 ];
	float	colors[ ( COLOR_WHEEL_RES + 1 ) * 2 * 4 ];
	float	colorScale;
	float	radius;
};

struct PolCord
{
	float angle;
	float radius;
};

class ofxColorPicker : public ofBaseDraws
{

public :
	
	 ofxColorPicker();
	~ofxColorPicker();

	void  update			();
	void  update			( ofEventArgs &e );
	void  exit				( ofEventArgs &e );

	void  setPos			( float x, float y );
	void  setSize			( float x, float y, float w, float h );
	
	void  draw				();
	void  draw				( float x, float y );
	void  draw				( float x, float y, float w, float h );

	float getWidth			();
	float getHeight			();
	
	void  show				();
	void  hide				();
	void  toggleShow		();
	bool  isShowing			();
	
	void  enable			();
	void  disable			();
	
	void  setColor			( const ofColor& c );
	ofColor getColor		();
	
	void  setColorScale		( float s );
	float getColorScale		();
	void  setColorRadius	( float r );
	float getColorRadius	();
	void  setColorAngle		( float a );
	float getColorAngle		();
	
private :

	void init				();
	bool checkDimensions	( int x, int y, int w, int h );

	void updateColor		();
	void updateColorScale	();
	
	void drawBackground		();
	void drawColorWheel		();
	void drawColorPoint		();
	void drawColorScaleBar	();
	void drawColorRect		();
	void drawDebug			();

	ofColor getCircularColor	( float angle, float radius, float scale );
	ofPoint getPoint			( float a, float r );
	PolCord getPolarCoordinate	( float x, float y );

	ofxMSAInteractiveObject colorWheel;
	ofxMSAInteractiveObject colorScaleBar;
	
	bool		bVisible;
	bool		bEnabled;
	
	ofColor		color;
	ofPoint		colorPoint;
	float		colorScale;
	float		colorRadius;				
	float		colorAngle;
	
	ofPoint		mousePoint;					// mouse interpolation.
	
	ofRectangle dimensions;
	int			colorWheelRadius;
	int			colorWheelPad;

	ofRectangle rectBackground;
	ofRectangle rectColorWheel;
	ofRectangle rectColorScaleBar;
	ofRectangle rectColorBox;
	
	ColorWheel	wheelLayer;
	Rectangle	rectScaleLayer;
};

#endif