#pragma once

#include "ofMain.h"
#include "ofxMSAFluid.h"
#include "ParticleSystem.h"
#include "OpticalField.h"

// comment this line out if you don't wanna use TUIO
// you will need ofxTUIO & ofxOsc
#define USE_TUIO		

// comment this line out if you don't wanna use the GUI
// you will need ofxSimpleGuiToo, ofxMSAInteractiveObject & ofxXmlSettings
// if you don't use the GUI, you won't be able to see the fluid parameters
#define USE_GUI		



#ifdef USE_TUIO
#include "ofxTuio.h"
#define tuioCursorSpeedMult				0.02f	// the iphone screen is so small, easy to rack up huge velocities! need to scale down 
#define tuioStationaryForce				0.001f	// force exerted when cursor is stationary
#endif


#ifdef USE_GUI 
#include "ofxSimpleGuiToo.h"
#endif

#define USE_OPTICAL_FIELD
#define USE_VIDEO
#define	USE_TIME_DISTORTION


class testApp : public ofSimpleApp{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);

	void windowResized(int w, int h);

	void addToFluid(float x, float y, float dx, float dy, bool addColor = true, bool addForce = true);

	bool				drawFluid;
	bool				drawParticles;
	bool				renderUsingVA;
	
	ofxMSAFluidSolver	fluidSolver;
	ofxMSAFluidDrawer	fluidDrawer;	
	
	ParticleSystem		particleSystem;
	
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
	
	
#ifdef USE_TUIO
	
	myTuioClient tuioClient;
	
#endif	
	
#ifdef USE_GUI 
	
	ofxSimpleGuiToo	gui;
	
#endif
	
#ifdef USE_OPTICAL_FIELD
	
	OpticalField	opticalField;
	int				camWidth;
	int				camHeight;
	
#endif	
	
#ifdef USE_VIDEO
	
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
	
#ifdef USE_TIME_DISTORTION
	
	ofTexture			timeDistTexture;
	unsigned char		*timeDistPixels;		
	
#endif
	
};

extern testApp *myApp;



