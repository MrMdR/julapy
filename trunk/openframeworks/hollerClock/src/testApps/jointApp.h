#ifndef JOINT_APP
#define JOINT_APP


#include "ofMain.h"
#include "ofxBox2d.h"

class jointApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void createJoint	();

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	//--------------------------------
	
	ofxBox2d				box2d;
	b2Body*					point;
	ofxBox2dCircle			c1;
	ofxBox2dCircle			c2;
	ofxBox2dJoint			joint;
};

#endif
