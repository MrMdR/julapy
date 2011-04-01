#ifndef NAWLZ_FISH_BOWL_APP
#define NAWLZ_FISH_BOWL_APP

#include "ofMain.h"

#ifndef TARGET_OF_IPHONE

#include "ofxSimpleGuiToo.h"

#include "NawlzFishBowl.h"

class NawlzFishBowlApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	bool				bDebug;
	
	NawlzFishBowl		nawlzFishBowl;
	
	ofImage				backgroundImage;
	ofImage				particleImage;
	ofImage				bowlImage;
	
	ofxSimpleGuiToo		gui;
};

#endif

#endif
