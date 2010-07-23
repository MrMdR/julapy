#ifndef _FILTERING_APP
#define _FILTERING_APP


#include "ofMain.h"
#include "ofxBox2d.h"

class filteringApp : public ofBaseApp
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
	
	//--------------------------------
	
	void createBounds	( b2Body* ground, float x, float y, float w, float h, int groupIndex );
	void createCircles	( vector<ofxBox2dCircle>& circles, int circlesNum, int groupIndex );
	void drawCircles	( vector<ofxBox2dCircle>& circles, int color );
	
	ofxBox2d				box2d;
	
	vector<ofxBox2dCircle>	blueCircles;
	vector<ofxBox2dCircle>	redCircles;
	
	b2Body*					ground1;
	b2Body*					ground2;

};

#endif
