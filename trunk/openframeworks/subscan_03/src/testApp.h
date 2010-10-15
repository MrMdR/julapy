#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSrt.h"

class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void addKey					( int key );
	void createString			();
	void updateVideoProgress	( float p );
	bool searchWord				( string word );
	
	string toUpperCase			( string str );
	string toLowerCase			( string str );
	string alphaClean			( string str );
	
	void drawProgressBar		();

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	ofTrueTypeFont		font;
	vector<int>			keys;
	string				keyString;
	string				subtitleStr;
	string				searchStr;
	bool				bSearchFound;
	
	ofxSrt				srt;
	ofVideoPlayer		video;
	ofRectangle			videoRect;
	float				videoProgress;
	
};

#endif
