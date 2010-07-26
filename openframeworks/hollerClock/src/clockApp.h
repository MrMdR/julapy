#ifndef _CLOCK_APP
#define _CLOCK_APP


#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxScreenGrabUtil.h"
#include "ofxSimpleGuiToo.h"
#include "Ball2d.h"
#include "ofxBox2d.h"
#include "Clock.h"


class clockApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void initClock		();
	void initGui		();
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	int						frameRate;
	bool					bDebug;
	
	ofxBox2d				box2d;
	ofxBox2dContactListener	contactListener;

	Clock					clock;
	
	ofTrueTypeFont			font;
	ofxScreenGrabUtil		screenGrabber;
	
	ofxSimpleGuiToo			gui;
};

#endif
