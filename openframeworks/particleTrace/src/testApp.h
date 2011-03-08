#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxTileSaver.h"
#include "ofxScreenGrabUtil.h"

#include "ParticleTrace.h"

class testApp : public ofBaseApp{

public:

	void setup	();
	void update	();
	void draw	();
	
	void initGui		();
	
	void keyPressed		(int key );
	void keyReleased	(int key );
	void mouseMoved		(int x, int y );
	void mouseDragged	(int x, int y, int button );
	void mousePressed	(int x, int y, int button );
	void mouseReleased	(int x, int y, int button );
	void windowResized	(int w, int h );

	bool				bDebug;
	bool				bStepMode;
	bool				bStepOne;
	
	ParticleTrace		pt;
	
	ofxTileSaver		tileSaver;
	ofxScreenGrabUtil	screenGrabber;
};

#endif
