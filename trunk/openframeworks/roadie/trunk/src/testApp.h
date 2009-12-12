#pragma once

#include "ofMain.h"
#include "ofxMultiTouch.h"
#include "ofxAccelerometer.h"
#include "ofxLocation.h"
#include "ofxiPhoneExtras.h"

#include "ofxOsc.h"

class testApp : public ofSimpleApp, public ofxMultiTouchListener, public ofxLocationListener  {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	bool checkOscTimer();
	void sendOscMessage();
	
	void drawData( vector<float> *data, int w, int h );
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	
	void locationChange ( ofxLocationCustomLocationData *data );
	void headingChange  ( ofxLocationCustomHeadingData  *data );
	
	ofxiPhoneKeyboard		*keyboard;
	ofxiPhoneCoreLocation	*coreLocation;

	ofRectangle		renderArea;
	
	ofTrueTypeFont	verdana;
	
	bool	hasGPS;
	
	int		resX;
	float	bandSize;
	int		bandMult;
	
	int		elapsedTime;
	
	string	oscHost;
	int		oscPort;
	int		oscTime;
	int		oscTimeout;
	
	ofxOscSender osc;
	
	float	speedValue;
	float	speedValueMax;
	float	speedValueNorm;
	
	int		locationTime;
	
	float	drawOffset;
	bool	drawVertical;
	
	vector<float>	tiltY;
	vector<float>	tiltX;
	vector<float>	speed;
	vector<float>	audio;
};

