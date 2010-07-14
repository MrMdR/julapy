#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "Tracker.h"
#include "Flock.h"
#include "Rocks.h"
#include "ofxCvBlob.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOsc.h"

#define MODE_BOIDS		1
#define MODE_TRACK		2
#define MODE_ROCKS		3

class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void initRenderArea		();
	void initTracker		();
	void initBoids			();
	void initRocks			();
	void initOsc			();
	void initBlendModes		();
	void addBlendMode		( GLuint srcBlend, GLuint dstBlend );
	void initGui			();
	
	void updateBlobs		();
	void updateBoids		();
	void updateRocks		();
	void updateOsc			();
	void updateRenderArea	();
	
	void drawTracking		();
	void drawBoids			();
	void drawRocks			();
	void drawRocksDebug		();
	void drawPeeps			();
	void drawBoidTexture	();
	
	void loadRockData		( string fileName = "rockData" );

	void keyPressed			( int key );
	void keyReleased		( int key );
	void mouseMoved			( int x, int y );
	void mouseDragged		( int x, int y, int button );
	void mousePressed		( int x, int y, int button );
	void mouseReleased		( int x, int y, int button );
	void windowResized		( int w, int h );

	bool					bDebug;
	bool					bFullScreen;
	bool					bRightMonitor;
	bool					bUseRockBlobs;
	bool					bUseTrackerBlobs;
	bool					bAddFoodForBoids;
	
	int						mode;

	ofRectangle				renderArea;
	ofRectangle				renderAreaWindow;
	ofRectangle				renderAreaFullScreen;
	ofRectangle				renderAreaRightMonitor;

	Flock					flock;
	Tracker					tracker;
	Rocks					rocks;
	
	vector<ofxCvBlob>		blobs;		// combination of rocks + people.
	
	ofVideoPlayer			boidVideo;
	vector<ofImage>			boidFrames;
	
	vector<string>			oscSenderHosts;
	vector<int>				oscSenderPorts;
	vector<ofxOscSender>	oscSenders;
	
	float					oscTreeVal;
	
	vector<GLuint>			blendModes;
	int						blendModeIndex;
};

#endif
