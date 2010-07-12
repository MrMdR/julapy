#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxVideoGrabber.h"
#include "ofxOpenCv.h"
#include "ofxContourUtil.h"
#include "ofxSimpleGuiToo.h"
#include "ofxCvWarper.h"

#include "Flock.h"

#define USE_VIDEO_INPUT				// comment out to use camera input.
#define USE_POINT_GREY_CAMERA		// comment out to use normal camera that does not require IIDC lib.

#define MODE_BOIDS		1
#define MODE_TRACK		2


class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void initRenderArea		();
	void initBoids			();
	
	void initVideo			();
	void initVideoGrabber	();
	void initVideoPlayer	();
	void initGui			();
	
	void updateVideoGrabber	();
	void updateVideoPlayer	();
	void updateTracking		();
	void updateBackground	();
	int  updateContours		();
	void updateCameraPos	( int x, int y );
	
	void updateBlobs		();
	void updateBoids		();
	void updateRenderArea	();
	
	void drawTracking		();
	void drawBoids			();
	void drawRocks			();
	void drawPeeps			();
	
	void loadRockData		( string fileName = "rockData" );

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	bool					bDebug;
	bool					bBoids;
	bool					bShiftDown;
	bool					bFullScreen;
	bool					bRightMonitor;
	bool					bUpdateCameraPos;
	
	int						mode;

	ofRectangle				renderArea;
	ofRectangle				renderAreaWindow;
	ofRectangle				renderAreaFullScreen;
	ofRectangle				renderAreaRightMonitor;
	
	int						modeTrackOffsetX;
	int						modeTrackOffsetY;
	
	//-- boids.
	
	Flock				flock;
	vector<ofxCvBlob>	blobs;		// combination of rocks + people.
	vector<ofxCvBlob>	rocks;
	vector<ofxCvBlob>	peeps;
	
	//-- opencv.
	
#ifdef USE_VIDEO_INPUT	
	ofVideoPlayer			videoPlayer;
#else
#ifdef USE_POINT_GREY_CAMERA	
	ofxVideoGrabber			videoGrabber;
#else
	ofVideoGrabber			videoGrabber;
#endif
#endif
	
	ofRectangle				videoLrgRect;
	ofRectangle				videoSmlRect;
	
	bool					isVideoNewFrame;
	
	ofxCvGrayscaleImage		videoLrgImage;
	ofxCvGrayscaleImage		videoSmlImage;
	
	//--
	
	ofxCvWarper				cvWarper;
	ofxCvGrayscaleImage		cvImageWarp;
	ofxCvGrayscaleImage		cvImageBg;
	ofxCvGrayscaleImage		cvImageDiff;
	ofxCvGrayscaleImage		cvImageThsh;
	
	int						threshold_1;
	int						threshold_2;
	int						blur;
	
	ofxCvContourFinder		contourFinder;
	ofxContourUtil			contourUtil;
	
	ofPoint					cameraPosition;
	float					cameraRadius;
};

#endif
