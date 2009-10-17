#ifndef _TEST_APP
#define _TEST_APP

#define USE_POINT_GREY_CAMERA

//#define USE_VIDEO_INPUT			// EITHER SELECT CAMERA OR VIDEO INPUT, NOT BOTH.
#define USE_CAMERA_INPUT		// EITHER SELECT CAMERA OR VIDEO INPUT, NOT BOTH.

//#define FLUID_WIDTH		120
#define FLUID_WIDTH		20

#include "ofMain.h"
#include "ofxVideoGrabber.h"
#include "ofxMSAFluid.h"
#include "OpticalField.h"
#include "ofxSimpleGuiToo.h"
#include "ofxScreenGrabUtil.h"

class testApp : public ofSimpleApp
{

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
	void initTimeDistortionForVideo();
	void initFluidForVideo();
	void initGui();
	
	void updateVideoGrabber();
	void updateVideoInput();
	void updateOpticalFieldFromCameraInput();
	void updateOpticalFieldFromVideoInput();
	void updateFluid();
	void updateVideo();
	void updateTimeDistortionForVideo();
	
	void drawFluidToVideoDimensions();
	void drawVideoSource();
	void drawCameraSourceForOpticalField();
	void drawTimeDistortionFromVideoSource();
	void drawTimeDistortionFromVideoSourceFullScreen();
	void drawDebugInfo();

	void updateRenderArea();
	
	void keyPressed( int key );
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );

	void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);

	ofRectangle			renderArea;
	ofRectangle			renderAreaWindow;
	ofRectangle			renderAreaFullScreen;
	ofRectangle			renderAreaRightMonitor;
	
	ofxScreenGrabUtil	screenGrabUtil;
	
	bool				bDebug;
	bool				bRightMonitor;
	
	ofxMSAFluidSolver	fluidSolver;
	ofxMSAFluidDrawer	fluidDrawer;
	float				fluidColorScale;
	
#ifdef USE_POINT_GREY_CAMERA
	ofxVideoGrabber 	videoGrabber;
#else
	ofVideoGrabber		videoGrabber;
#endif
	
	bool				isVideoGrabberNewFrame;
	int					camWidth;
	int					camHeight;
	int					camWidthHalf;
	int					camHeightHalf;
	
	ofxSimpleGuiToo		gui;

	OpticalField		opticalField;
	float				interactionScale;
	
#ifdef USE_VIDEO_INPUT
	
	ofVideoPlayer		videoInput;
	int					videoInputWidth;
	int					videoInputHeight;
	float				videoInputPosition;
	
#endif
	
	ofVideoPlayer		videoPlayer;
	ofTexture			videoPlayerTexture;
	unsigned char		*videoPlayerTexturePixels;
	int					videoPlayerWidth;
	int					videoPlayerHeight;
	int					videoPlayerPixelsPerFrame;
	
	unsigned char		*frameBuffer;
	int					frameBufferTotal;
	int					frameBufferPlayIndex;
	
	ofTexture			timeDistTexture;
	unsigned char		*timeDistPixels;
	
};

#endif