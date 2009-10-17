#ifndef _TEST_APP
#define _TEST_APP

#define USE_POINT_GREY_CAMERA

//#define USE_VIDEO_INPUT			// USE ONLY VIDEO OR CAMERA ONE AT A TIME.
#define USE_CAMERA_INPUT		// USE ONLY VIDEO OR CAMERA ONE AT A TIME.

#define USE_VIDEO_OUTPUT		// USE ONLY VIDEO OR CAMERA ONE AT A TIME.
//#define USE_CAMERA_OUTPUT		// USE ONLY VIDEO OR CAMERA ONE AT A TIME.

//#define USE_VIDEO_SAVER
#define SHOW_DEBUG

#include "ofMain.h"
#include "ofxVideoGrabber.h"
#include "ofxMSAFluid.h"
#include "OpticalField.h"
#include "ofxSimpleGuiToo.h"

#ifdef USE_VIDEO_SAVER
	#include "ofQtVideoSaver.h"
#endif

class testApp : public ofSimpleApp{
public:
	void setup();
	void update();
	void draw();
	
	void initRenderArea();
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

	void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);

	void toggleFullScreen();
	
	ofRectangle		renderArea;
	ofRectangle		renderAreaWindow;
	ofRectangle		renderAreaFullScreen;
	ofRectangle		renderAreaRightMonitor;
	
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
	
#ifdef USE_POINT_GREY_CAMERA
	ofxVideoGrabber 	videoGrabber;
#else
	ofVideoGrabber		videoGrabber;
#endif
	
	bool				isVideoGrabberNewFrame;
	ofxCvColorImage		videoGrabberImage;
	int					camWidth;
	int					camHeight;
	int					camWidthHalf;
	int					camHeightHalf;
	
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
	
	ofxSimpleGuiToo	gui;

	OpticalField		opticalField;
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

#endif