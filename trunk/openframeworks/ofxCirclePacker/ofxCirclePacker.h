/*
 *  ofxCirclePacker.h
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 22/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_CIRCLE_PACKER_H_
#define _OFX_CIRCLE_PACKER_H_

#include "ofMain.h"
#include "ofxColorUtils.h"
#include <fstream>

struct Circle 
{
	ofPoint		loc;
	float		radius;
	float		growth;
	int			color;
	int			lifeCount;
	bool		alive;
};

class ofxCirclePacker
{

public :
	
	 ofxCirclePacker ();
	~ofxCirclePacker ();
	
	void setColorBoundsImage	( ofImage *image, ofRectangle *imageRect );
	void setColorMapImage		( ofImage *image, ofRectangle *imageRect );
	void setCircleRadiusMin		( float radiusMin );
	void setCircleRadiusMax		( float radiusMax );
	void setCircleGap			( float gap );
	void setCircleDeathGap		( float gap );
	void setCircleDeathColor	( int color );
	void setCircleColorBounds	( bool useColorBounds );
	void togglePause			();
	
	void addCircle		( int color = 0xFFFFFF );
	void update			();
	void draw			();
	void reset			();
	
	void writeToFile	( string filename = "circle_data" );
	void loadFromFile	( string filename = "circle_data" );
	
	vector<Circle> * getCircles	();
	
private :

	vector<Circle>	circles;
	
	ofImage			*circleColorBoundsImage;
	unsigned char	*circleColorBoundsImagePixels;
	ofRectangle		circleColorBoundsImageRect;
	bool			useCircleColorBoundsImage;
	
	ofImage			*circleColorMapImage;
	unsigned char	*circleColorMapImagePixels;
	ofRectangle		circleColorMapImageRect;
	bool			useCircleColorMapImage;
	
	float			circleRadiusMin;
	float			circleRadiusMax;
	float			circleGap;
	float			circleDeathGap;
	int				circleDeathColor;
	bool			circleColorBounds;
	
	bool			bPaused;
	
	void  checkCircleCollision	();
	void  checkCircleImage		();
	void  removeInvalidCircles	();
	
	int	  getImageColor			( int x, int y, unsigned char *pixels, ofRectangle *rect );
	
	float distance				( float x1, float y1, float x2, float y2 );
	float fastDistance			( float x1, float y1, float x2, float y2 );
	bool  circleCollision		( float x1, float y1, float x2, float y2, float radius1, float radius2 );
	bool  fastCircleCollision	( float x1, float y1, float x2, float y2, float radius1, float radius2 );
};

#endif