#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ColorCycle.h"
#include "ColorSound.h"
#include "FooterBar.h"
#include "SplashScreen.h"
#include "InfoScreen.h"

class testApp : public ofxiPhoneApp 
{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
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
	
	ColorCycle*		cc;
	ColorSound*		sounds;
	FooterBar*		footer;
	SplashScreen*	splashScreen;
	InfoScreen*		infoScreen;
	
	ofPoint			lastTouch;
	ofPoint			lastTouchMoved;
	int				lastTouchId;
	int				lastTouchCount;
	
	bool			upsideDown;
};


