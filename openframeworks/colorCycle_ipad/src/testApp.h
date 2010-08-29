#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ColorCycle.h"
#include "ofxColorPicker.h"
#include "FooterBar.h"
#include "InfoScreen.h"
#include "ofxALSoundPlayer.h"
#include "ColorConstants.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void initSounds			();
	void createSounds		( const vector<string>& fileNames, vector<ofxALSoundPlayer*>& sounds, bool loop = false );
	void createNumOfSounds	( const vector<string>& fileNames, vector<ofxALSoundPlayer*>& sounds, bool loop = false, int numOfSounds = CIRCLES_MAX );
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
	FooterBar		footer;

	InfoScreen		infoScreen;
	ofImage*		splashScreen;
	
	ofPoint			lastTouch;
	ofPoint			lastTouchMoved;
	int				lastTouchId;
	int				lastTouchCount;
	
	bool			upsideDown;
	
	vector<ofxALSoundPlayer*>	spBackground;
	vector<ofxALSoundPlayer*>	spMeshDrag;
	vector<ofxALSoundPlayer*>	spPointAdd;
	vector<ofxALSoundPlayer*>	spPointRemove;
	vector<ofxALSoundPlayer*>	spPointCollide;
	vector<ofxALSoundPlayer*>	spPointShuffle;
};


