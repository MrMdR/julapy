#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"

#include "Flock.h"

#include <fstream>

class testApp : public ofBaseApp
{

public :
	
	void setup			();
	void update			();
	void draw			();

	void initRenderArea	();
	void initBoids		();
	void initGui		();
	
	void updateBoids		();
	void updateRenderArea	();
	
	void drawBoids		();
	void drawBlobs		();
	
	void loadRockData	( string fileName = "rockData" );

	void keyPressed		(int key);
	void keyReleased	(int key);
	void mouseMoved		(int x, int y );
	void mouseDragged	(int x, int y, int button);
	void mousePressed	(int x, int y, int button);
	void mouseReleased	(int x, int y, int button);
	void windowResized	(int w, int h);
	
	bool				bDebug;
	bool				bShiftDown;
	bool				bRightMonitor;
	
	ofRectangle			renderArea;
	ofRectangle			renderAreaWindow;
	ofRectangle			renderAreaFullScreen;
	ofRectangle			renderAreaRightMonitor;
	
	Flock				flock;
	
	vector<ofxCvBlob>	blobs;
};

#endif
