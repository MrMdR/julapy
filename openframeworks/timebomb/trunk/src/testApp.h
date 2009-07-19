#pragma once

#include "ofMain.h"
#include "ofxMSAFluid.h"
#include "OpticalField.h"
#include "ofQtVideoSaver.h"

//#define USE_TUIO
#ifdef USE_TUIO
	#include "ofxTuio.h"
	#define tuioCursorSpeedMult				0.02f	// the iphone screen is so small, easy to rack up huge velocities! need to scale down 
	#define tuioStationaryForce				0.001f	// force exerted when cursor is stationary
#endif

#define USE_GUI		
#ifdef USE_GUI 
	#include "ofxSimpleGuiToo.h"
#endif

//#define USE_VIDEO_INPUT			// USE ONLY VIDEO OR CAMERA ONE AT A TIME.
#define USE_CAMERA_INPUT		// USE ONLY VIDEO OR CAMERA ONE AT A TIME.

#define USE_VIDEO_OUTPUT		// USE ONLY VIDEO OR CAMERA ONE AT A TIME.
//#define USE_CAMERA_OUTPUT		// USE ONLY VIDEO OR CAMERA ONE AT A TIME.

//#define USE_VIDEO_SAVER
//#define SHOW_DEBUG


class testApp : public ofSimpleApp{
public:
	void setup();
	void update();
	void draw();
	
	void initVideoGrabber();
	void initVideoInput();
	void initOpticalFieldForCameraInput();
	void initOpticalFieldForVideoInput();
	void initVideoOutput();
	void initCameraOutput();
	void initTimeDistortionForVideo();
	void initTimeDistortionForCamera();
	void initFluid();
	void initFluidForVideo();
	void initFluidForCamera();
	void initGui();
	void initVideoSaver();
	
	void updateTuio();
	void updateVideoGrabber();
	void updateVideoInput();
	void updateOpticalFieldFromCameraInput();
	void updateOpticalFieldFromVideoInput();
	void updateFluid();
	void updateVideo();
	void updateCamera();
	void updateTimeDistortionForVideo();
	void updateTimeDistortionForCamera();
	
	void drawFluidFullScreen();
	void drawFluidToVideoDimensions();
	void drawFluidToCameraDimensions();
	void drawVideoSource();
	void drawCameraSource();
	void drawCameraSourceForOpticalField();
	void drawTimeDistortionFromVideoSource();
	void drawTimeDistortionFromVideoSourceFullScreen();
	void drawTimeDistortionFromCameraSource();
	void drawTimeDistortionFromCameraSourceFullScreen();
	void drawToVideoSaver();
	void drawDebugInfo();
	
	void keyPressed( int key );
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );

	void windowResized( int w, int h );

	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);

	bool				drawFluid;
	bool				renderUsingVA;
	
	ofxMSAFluidSolver	fluidSolver;
	ofxMSAFluidDrawer	fluidDrawer;
	float				fluidColorScale;
	
	int					pmouseX, pmouseY;
	
	// cache these for slightly better performance
	struct {
		int				width;
		int				height;
		float			invWidth;
		float			invHeight;
		float			aspectRatio;
		float			aspectRatio2;
	} window;
	
	ofVideoGrabber		videoGrabber;
	bool				isVideoGrabberNewFrame;
	
#ifdef USE_VIDEO_SAVER
	
	ofQtVideoSaver		videoSaver;
	ofImage				videoSaverImage;
	int					videoSaverWidth;
	int					videoSaverHeight;
	bool				videoSaverRecording;
	string				videoSaverPath;

#endif
	
#ifdef USE_TUIO
	
	myTuioClient tuioClient;
	
#endif	
	
#ifdef USE_GUI 
	
	ofxSimpleGuiToo	gui;
	
#endif
	
	OpticalField		opticalField;
	int					camWidth;
	int					camHeight;
	float				interactionScale;
	
#ifdef USE_VIDEO_INPUT
	
	ofVideoPlayer		videoInput;
	int					videoInputWidth;
	int					videoInputHeight;
	float				videoInputPosition;
	
#endif
	
#ifdef USE_VIDEO_OUTPUT
	
	ofVideoPlayer		videoPlayer;
	ofTexture			videoPlayerTexture;
	unsigned char		*videoPlayerTexturePixels;
	int					videoPlayerWidth;
	int					videoPlayerHeight;
	int					videoPlayerPixelsPerFrame;
	
	unsigned char		*frameBuffer;
	int					frameBufferTotal;
	int					frameBufferPlayIndex;
	
#endif
	
#ifdef USE_CAMERA_OUTPUT
	
	ofTexture			videoGrabberTexture;
	unsigned char		*videoGrabberTexturePixels;
	int					videoGrabberWidth;
	int					videoGrabberHeight;
	int					videoGrabberPixelsPerFrame;
	
	unsigned char		*frameBuffer;
	int					frameBufferLimit;
	int					frameBufferCount;
	int					frameBufferIndex;
	
#endif
	
	ofTexture			timeDistTexture;
	unsigned char		*timeDistPixels;
	
};

extern testApp *myApp;



