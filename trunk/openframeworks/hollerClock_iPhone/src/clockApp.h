#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxALSoundPlayer.h"

#include "ofxBox2d.h"
#include "Clock.h"

#define DEVICE_IPAD		0
#define DEVICE_IPHONE	1

class clockApp : public ofxiPhoneApp {
	
public:
	
	void setup	();
	void update	();
	void draw	();
	void exit	();
	
	void initClock		();
	
	void touchDown		(ofTouchEventArgs &touch);
	void touchMoved		(ofTouchEventArgs &touch);
	void touchUp		(ofTouchEventArgs &touch);
	void touchDoubleTap	(ofTouchEventArgs &touch);

	void lostFocus					();
	void gotFocus					();
	void gotMemoryWarning			();
	void deviceOrientationChanged	(int newOrientation);
	
	//------------------------------------------

	int						deviceType;
	int						frameRate;
	
	ofxBox2d				box2d;
	ofxBox2dContactListener	contactListener;
	
	Clock					clock;
	
	ofTrueTypeFont			font;
	
	ofxALSoundPlayer		secTwoSound;
	ofxALSoundPlayer		secOneSound;
};


