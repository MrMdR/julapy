#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ColorCycle.h"
#include "ColorSound.h"
#include "ColorConstants.h"
#include "ofxBox2d.h"
 
class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	int				frameRate;
	
	ofxBox2d		box2d;
	ColorCycle		cc;
	ColorSound		cs;
};

#endif
