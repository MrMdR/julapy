#ifndef NAWLZ_QUESTIONS_APP
#define NAWLZ_QUESTIONS_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

#include "NawlzQuestions.h"

class NawlzQuestionsApp : public ofBaseApp
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
    
    NawlzQuestions      app;
    
};

#endif
