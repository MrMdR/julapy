/*
 *  Clock.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 15/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CLOCK_H
#define CLOCK_H

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxVec2f.h"
#include "ofxContourUtil.h"
#include "ClockCircle.h"

#define CLOCK_MODE_1		1
#define CLOCK_MODE_2		2
#define RAY_BLOB_HI_RES		200
#define RAY_BLOB_LO_RES		10

class Clock : public ofxBox2dContactReceiver
{
public :
	
	 Clock();
	~Clock();
	
	void box2dContactEventHandler ( const b2ContactPoint* point );
	
	void  setBox2d			( ofxBox2d *box2d );
	void  setSize			( ofRectangle &size );
	void  setSize			( int w, int h );
	void  setTimeFont		( ofTrueTypeFont *font );
	void  setGravity		( float x, float y );
	void  setForceScale		( float f );
	
	void  createBounds		();
	void  createCircles		();
	void  createCircle		( vector<ClockCircle*> &circlesVec, int numOfCircle, float radius, int color = 0xFFFFFF, float lineX = 0 );
	float areaToRadius		( float area );
	void  createSoftBody	();
	
	void setup				();
	void update				( int hrs, int min, int sec );
	void draw				();
	
	void initModeOne		();
	void initModeTwo		();
	void updateText			();
	void updateForces		();
	void updateForcesVec	( vector<ClockCircle*> &circlesVec, int count );
	
	void addCenterForce		( ClockCircle& circle );
	void pushFromCenter		( ClockCircle& circle );
	void floatUp			( ClockCircle& circle );
	void lineUp				( ClockCircle& circle );
	
	void updateRayBlob		();
	void updateConvexBlob	();
	
	void toggleClockMode	();
	
	void drawCircles		( vector<ClockCircle*>& circles );
	void drawCircle			( ClockCircle &circle );
	void drawCircleLines	( vector<ClockCircle*>& circles );
	void drawCircleLine		( ClockCircle &circle );
	void drawTime			();
	void drawRayCasts		();
	void drawRayBlob		();
	void drawConvexBlob		( const vector<ofPoint>& points );
	
	vector<ClockCircle*>	hrsOne;
	vector<ClockCircle*>	hrsTwo;
	vector<ClockCircle*>	minOne;
	vector<ClockCircle*>	minTwo;
	vector<ClockCircle*>	secOne;
	vector<ClockCircle*>	secTwo;
	vector<ClockCircle*>	circlesAll;
	vector<ClockCircle*>	circlesActive;
	vector<ClockCircle*>	circlesInactive;
	
	int						clockMode;
	
	int						hrsOneCount;
	int						hrsTwoCount;
	int						minOneCount;
	int						minTwoCount;
	int						secOneCount;
	int						secTwoCount;
	
	int						hrsOneTotal;
	int						hrsTwoTotal;
	int						minOneTotal;
	int						minTwoTotal;
	int						secOneTotal;
	int						secTwoTotal;
	
	int						hrsOneX;
	int						hrsTwoX;
	int						minOneX;
	int						minTwoX;
	int						secOneX;
	int						secTwoX;

	int						hrsOneM1X;
	int						hrsTwoM1X;
	int						minOneM1X;
	int						minTwoM1X;
	int						secOneM1X;
	int						secTwoM1X;

	int						hrsOneM2X;
	int						hrsTwoM2X;
	int						minOneM2X;
	int						minTwoM2X;
	int						secOneM2X;
	int						secTwoM2X;
	
	int						screenWidth;
	int						screenHeight;
	int						screenMinLength;
	int						screenMaxLength;
	int						screenTotal;
	ofxVec2f				screenCenter;
	
	ofTrueTypeFont*			font;
	
	ofxBox2d*				box2d;
	b2Body*					ground;
	ofxBox2dSoftBody*		softBody;
	
	ofxContourUtil			contourUtil;
	vector<ofPoint>			convexBlobInner;
	vector<ofPoint>			convexBlobOuter;
	
	ofPoint					rayBlob[ RAY_BLOB_LO_RES ];
	ofPoint					gravity;
	float					forceScale;
	
	int						forceCenterPull;
	int						forceCenterPush;
	float					rayBlobPad;
	float					rayBlobEase;
	
};

#endif