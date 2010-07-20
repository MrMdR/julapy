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
#include "ofxVectorMath.h"
#include "ClockCircle.h"

#define CLOCK_MODE_1	1
#define CLOCK_MODE_2	2

class Clock
{

public :
	
	 Clock();
	~Clock();
	
	void  setBox2d			( ofxBox2d *box2d );
	void  setSize			( ofRectangle &size );
	void  setSize			( int w, int h );
	void  setTimeFont		( ofTrueTypeFont *font );
	void  createCircles		();
	void  createCircle		( vector<ClockCircle*> &circlesVec, int numOfCircle, float radius, int color = 0xFFFFFF, int lineX = 0 );
	float areaToRadius		( float area );
	
	void setup				();
	void update				();
	void draw				();
	
	void setForM1			();
	void setForM2			();
	void updateTime			();
	void updateTimeX		();
	void updateForcesM1		();
	void updateForcesM2		();
	void updateForcesM1b	( vector<ClockCircle*> &circlesVec, int count );
	void updateForcesM2b	( vector<ClockCircle*> &circlesVec, int count );
	void pullToCenter		( ClockCircle& circle );
	void pushFromCenter		( ClockCircle& circle );
	void floatUp			( ClockCircle& circle );
	void lineUp				( ClockCircle& circle );
	
	void toggleClockMode	();
	
	void drawCircles		();
	void drawCircle			( ClockCircle &circle );
	void drawTime			();
	void drawRayCasts		();
	
	vector<ClockCircle*>	hrsOne;
	vector<ClockCircle*>	hrsTwo;
	vector<ClockCircle*>	minOne;
	vector<ClockCircle*>	minTwo;
	vector<ClockCircle*>	secOne;
	vector<ClockCircle*>	secTwo;
	vector<ClockCircle*>	circlesAll;
	
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
	
	ofTrueTypeFont*			font;
	
	ofxBox2d*				box2d;
	
};

#endif