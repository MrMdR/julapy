#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ColorCycle.h"
#include "ofxColorPicker.h"
#include "FooterBar.h"
#include "InfoScreen.h"
#include "ofxALSoundPlayer.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawSplashScreen	();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	
	void checkLastTouch ();

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);

	int				frameRate;
	
	ColorCycle		cc;
	FooterBar*		footer;
	
	ofImage*		splashScreen;
	InfoScreen		infoScreen;
	
	ofPoint			lastTouch;
	ofPoint			lastTouchMoved;
	int				lastTouchId;
	int				lastTouchCount;
	
	bool			upsideDown;
};


