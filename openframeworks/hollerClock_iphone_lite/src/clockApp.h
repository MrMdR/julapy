#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxALSoundPlayer.h"
#include "ofxFlash.h"
#include "Quad.h"

#include "ofxBox2d.h"
#include "Clock.h"
#include "Background.h"

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
	
	void loadImageToTexture	( string imagePath, ofTexture& tex );
	
	void touchDown		( int x, int y, int id );
	void touchMoved		( int x, int y, int id );
	void touchUp		( int x, int y, int id );
	void touchDoubleTap	( int x, int y, int id );

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
	
	UIImage*				image;
	string					imagePath;
	ofRectangle				texFsSize;
	ofTexture				texBg;
	ofTexture*				texCells;
	int						texCellsNum;
	ofTexture				texInfo;
	ofTexture				texMembrane;
	ofTexture*				texDigits;
	int						texDigitsNum;
	
	float					rot;
	float					rotTime;
	float					rotTimeTotal;
	bool					bFlipLeft;
	bool					bFlipRight;
	
	ofxFlashStage*			stage;
	ofxFlashXFL				xfl;
	
	Background*				bg;
};


