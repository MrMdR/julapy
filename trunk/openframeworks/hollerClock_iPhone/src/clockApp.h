#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxALSoundPlayer.h"
#include "Quad.h"
#include "ofxFlash.h"

#include "ofxBox2d.h"
#include "Clock.h"
#include "FooterBar.h"

#define DEVICE_IPAD		0
#define DEVICE_IPHONE	1

class clockApp : public ofxiPhoneApp {
	
public:
	
	void setup	();
	void update	();
	void draw	();
	void exit	();
	
	void initClock		();
	
	void flipLeft		();
	void flipRight		();
	
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
	ofRectangle				screenSize;
	
	ofxBox2d				box2d;
	ofxBox2dContactListener	contactListener;
	
	Clock					clock;
	
	ofxALSoundPlayer		secTwoSound;
	ofxALSoundPlayer		secOneSound;
	
	ofImage					image;
	ofRectangle				texFsSize;
	ofTexture				texBg;
	ofTexture*				texCells;
	int						texCellsNum;
	ofTexture				texInfo;
	ofTexture				texMembrane;
	ofTexture*				texDigits;
	int						texDigitsNum;
	ofxMovieClip			texCellAnim;
	
	float					rot;
	float					rotTime;
	float					rotTimeTotal;
	bool					bFlipLeft;
	bool					bFlipRight;
	
	FooterBar*				footer;
};


