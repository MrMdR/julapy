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
	int		id;
	ofPoint	loc;
	float	radius;
	float	radiusNext;
	float	growth;
	int		color;
	int		lifeCount;
	bool	alive;
	bool	bUnderMinRadius;
	
	vector<Circle*>	neighbours;
};

class ofxCirclePacker
{

public :
	
	 ofxCirclePacker ();
	~ofxCirclePacker ();
	
	void setColorBoundsImage	( ofImage *image, const ofRectangle& imageRect );
	void setColorMapImage		( ofImage *image, const ofRectangle& imageRect );
	void setCircleRadiusMin		( float radiusMin );
	void setCircleRadiusMax		( float radiusMax );
	void setCircleGap			( float gap );
	void setCircleDeathGap		( float gap );
	void setCircleDeathColor	( int color );
	void setCircleColorBounds	( bool useColorBounds );

	void addCircles			( int numOfCircles, int color = 0xFFFFFF );
	void addCircle			( int color = 0xFFFFFF );
	
	void update			();
	void draw			();
	void reset			();
	
	vector<Circle>&		getCircles();
	
	void writeToFile	( string filename = "circle_data" );
	void loadFromFile	( string filename = "circle_data" );
	
private :

	vector<Circle>	circles;
	
	int				circleIdCount;
	int				circleIdLimit;
	
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
	
	void  findNeighbours						( Circle& circle );
	bool  checkCircleCollisionWithAll			( Circle& circle );
	bool  checkCircleCollisionWithNeighbours	( Circle& circle );
	bool  checkCircleCollision					( Circle& circle1, Circle& circle2 );
	bool  checkCircleImage						( Circle& circle );
	
	int	  getImageColor			( int x, int y, unsigned char *pixels, const ofRectangle& rect );
	
	float distance				( const ofPoint& p1, const ofPoint& p2 );
	float fastDistance			( const ofPoint& p1, const ofPoint& p2 );
	bool  circleCollision		( const ofPoint& p1, const ofPoint& p2, float radius1, float radius2 );
	bool  fastCircleCollision	( const ofPoint& p1, const ofPoint& p2, float radius1, float radius2 );
};

#endif