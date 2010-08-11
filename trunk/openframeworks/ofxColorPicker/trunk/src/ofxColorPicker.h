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
#define		COLOR_PICKER_DEFAULT_HEIGHT				100

#define		COLOR_PICKER_MODE_CIRLCE_ROTATION		0
#define		COLOR_PICKER_MODE_RANDOM_WALK			1
#define		COLOR_PICKER_MODE_MOUSE					2

class ofxColorPicker : public ofBaseDraws
{

public :
	
	 ofxColorPicker();
	~ofxColorPicker();

	void  update			( ofEventArgs &e );
	void  update			();
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
	
	void  setMode			( int m );
	void  setRandomColor	();
	
	void  setColor			( const ofColor& c );
	ofColor getColor		();
	
	void  setColorScale		( float s );
	void  setColorRadius	( float r );
	void  setColorAngle		( float a );
	void  addToColorAngle	( float a );
	
	void  setCircularUpperBounds		( float value );
	void  setCircularLowerBounds		( float value );
	void  setCircularSpeed				( float value );
	void  setCircularOscillationSpeed	( float value );
	
	void  setRandomWalkVelocityMax		( float value );
	void  setRandomWalkVelocityEase		( float value );
	
private :

	void  init();
	
	bool  checkPos( int x, int y );
	bool  checkSize( int w, int h );
	
	void  updateColorScale();
	
	void  initCircularInterpolation();
	void  updateCircularInterpolation();
	
	void  initRandomWalkInterpolation();
	void  updateRandomWalkInterpolation();
	
	void  initMouseInterpolation();
	void  updateMouseInterpolation();
	
	void  drawBackground( int w, int h );
	void  drawColorWheel( int w, int h );
	void  drawColorPoint( int w, int h );
	void  drawColorScaleBar( int w, int h );
	void  drawColorRect( int w, int h );

	void  getCircularColor( float angle, float radius, float scale, ofColor *c );
	void  getPoint( float a, float r, ofPoint *p );
	void  getAngleCoordinates( float x, float y, float *a, float *r );

	ofxMSAInteractiveObject colorWheel;
	ofxMSAInteractiveObject colorScaleBar;
	
	bool		visible;
	
	ofColor		color;
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