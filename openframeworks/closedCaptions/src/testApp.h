#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSrt.h"
#include "SpeachBubble.h"

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();

	void addSpeachBubble		( string bubbleText );
	void playOutSpeachBubbles	();
	void updateSpeachBubbles	();
	void drawSpeachBubbles		();
	void drawSpeachBubble		( SpeachBubble& bubble );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	ofxSrt					srt;
	ofVideoPlayer			video;
	string					subtitleStr;
	ofTrueTypeFont			fontCooperBlack;
	ofTrueTypeFont			fontBubble;
	vector<SpeachBubble>	bubbles;
	
};

#endif
