#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxTesseract.h"

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();
	
	void initGui ();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofxTesseract			tess;
	string					tessStr;
	
	ofTrueTypeFont			fontCooperBlack;

	ofImage					image;
	
};

#endif
