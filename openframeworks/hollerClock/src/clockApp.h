#ifndef _CLOCK_APP
#define _CLOCK_APP


#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxScreenGrabUtil.h"
#include "ofxSimpleGuiToo.h"
#include "ofxVideoSaver.h"
#include "ofxFlash.h"

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
	bool					bFullScreen;
	
	ofRectangle				screenRect;
	ofRectangle				standardRect;
	
	ofxBox2d				box2d;
	ofxBox2dContactListener	contactListener;

	Clock					clock;
	
	ofTrueTypeFont			font1;
	ofTrueTypeFont			font2;
	string					timeStr;
	
	ofxScreenGrabUtil		screenGrabber;
	
	ofxSimpleGuiToo			gui;
	
	ofImage					screenImage;
	ofxVideoSaver			videoSaver;
	
	ofSoundPlayer			secTwoSound;
	ofSoundPlayer			secOneSound;
	
	ofImage					image;
	ofRectangle				texFsSize;
	ofTexture				texBg;
	ofTexture*				texCells;
	int						texCellsNum;
	ofxMovieClip			texCellAnim;
	ofTexture*				texLines;
	int						texLinesNum;
	ofTexture				texInfo;
	ofTexture				texMembrane;
	ofTexture*				texDigits;
	int						texDigitsNum;
	
};

#endif
