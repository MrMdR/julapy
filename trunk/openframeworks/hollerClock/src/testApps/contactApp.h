#ifndef CONTACT_APP
#define CONTACT_APP

#include "ofMain.h"
#include "ofxBox2d.h"


class MyContactListener : public b2ContactListener
{
public:
	void Add(const b2ContactPoint* point);		
	void Remove(const b2ContactPoint* point);
};

//--------------------------------------------------------------

class CustomBall : public ofxBox2dCircle
{
public:
	void update	();
	void draw	();
	
	int r;
	int g;
	int b;
};

//--------------------------------------------------------------

class contactApp : public ofSimpleApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void box2dContactEventHandler	( const b2ContactPoint* point );
	
	void keyPressed		(int key);
	void keyReleased	(int key);
	void mousePressed	(int x, int y, int button);
	void mouseMoved		(int x, int y );
	void mouseDragged	(int x, int y, int button);
	void mouseReleased	(int x, int y, int button);

	void windowResized	(int w, int h);
	
	ofxBox2d					box2d;	
	vector<CustomBall>			balls;
	MyContactListener			contacts;
};

#endif