#ifndef NAWLZ_INFODEL_APP
#define NAWLZ_INFODEL_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

#include "NawlzInfodel.h"

class NawlzInfodelApp : public ofBaseApp
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
    
    ofImage             img;
	
	ofxSimpleGuiToo		gui;
    
    NawlzInfodel        app;
    
};

#endif
