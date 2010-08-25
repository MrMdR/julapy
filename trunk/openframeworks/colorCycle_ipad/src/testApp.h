#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ColorCycle.h"
#include "ofxColorPicker.h"
#include "FooterBar.h"

class testApp : public ofxiPhoneApp {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);

	int				frameRate;
	
	ColorCycle		cc;
	FooterBar*		footer;
	
	ofTouchEventArgs	lastTouch;
	bool				bTouchDown;
	int					touchCount;
};


