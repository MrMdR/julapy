/*
 *  ClockCircle.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLOCK_CIRCLE_H
#define CLOCK_CIRCLE_H

#include "ofxBox2d.h"

#define POINT_RES	20

struct ClockCircleTrg 
{
	ofPoint p1;
	ofPoint p2;
	ofPoint p3;
	float	rotation;
	float	rotationInc;
	ofColor	color;
};

class ClockCircle : public ofxBox2dCircle
{

public :
	
	 ClockCircle		( float radius = 0.001, int color = 0xFFFFFF );
	~ClockCircle		();

	void setSize		( ofRectangle &size );
	void setSize		( int w, int h );
	void setTexture		( ofTexture* tex );
	void setForceScale	( float scale );
	void setup			( b2World * b2dworld, float x, float y, float size, bool isFixed=false );

	void init			();
	void update			();
	void draw			();
	
	void createCenterJoint	();
	void createOuterJoint	();
	void createLineupJoint	();
	void createJoint		( const ofPoint& p );
	void createJoint		( float x = 0, float y = 0, float length = 0 );
	void destroyJoint		();
	bool hasJoint			();
	bool hasCenterJoint		();
	bool hasOuterJoint		();
	bool hasLineupJoint		();
	
	void drawCircles		();
	void drawTexture		();
	void drawTriangles		();
	
	int			screenWidth;
	int			screenHeight;
	
	int			pointsTotal;
	ofPoint		points1[ POINT_RES ];
	ofPoint		points2[ POINT_RES ];
	
	float		radius;
	float		rotation;
	int			colorHex;
	ofColor		colorTrgt;
	ofColor		colorCurr;
	
	ofPoint		lineUpPoint;
	
	int			spinDir;
	float		spinFrc;
	
	float		forceScale;
	
	bool		active;
	
	ofPoint		eye;
	
	b2Body*				point;
	b2DistanceJoint*	joint;
	
	bool		bCenterJoint;
	bool		bOuterJoint;
	bool		bLineupJoint;
	
	vector<ClockCircleTrg>	triangles;
	
	ofTexture*	tex;
	
};

#endif