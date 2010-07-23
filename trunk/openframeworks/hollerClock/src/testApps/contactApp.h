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

class contactApp : public ofSimpleApp{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased (int key);
	void mousePressed(int x, int y, int button);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	void windowResized(int w, int h);
	
	// box2d system
	ofxBox2d		welt;	
	ofxBox2dCircle	ball;
	ofxBox2dRect	box;

	
	MyContactListener	contacts;
	vector <b2Vec2> contact_points;
	vector <b2Vec2> contact_velocities;
	
	
};

#endif

