#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
//#include "FluidTexture.h"
#include "ofxMSAFluidSolver.h";
#include "MotionDetection.h";

class testApp : public ofBaseApp
{
	public:
	
		void setup();
		void initVideoPlayer();
		void initFluid();
		void initTemporalDistortion();
		void initMotionDetection();
		
		void update();
		void updateVideoPlayer();
		void updateFluid();
		void updateTemporalDistortion();
		void updateMotionDetection();
	
		void draw();
		void drawVideoPlayer();
		void drawFluid();
		void drawTemporalDistortion();
		void drawMotionDetection();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		Boolean				showVideoSource;
		Boolean				showFluid;
		Boolean				showTemporalDistortion;
		Boolean				showMotionDetection;
	
		ofVideoPlayer		vp;
		ofTexture			vpTexture;
		unsigned char *		vpTexturePixels;
		int					vpWidth;
		int					vpHeight;
		int					vpPixelsPerFrame;
		unsigned char * 	frameBuffer;
		int					frameBufferTotal;
		int					frameBufferPlayIndex;
	
//		FluidTexture		fluid;
		ofxMSAFluidSolver	fluid;
		ofTexture			fluidTexture;
		unsigned char *		fluidPixels;
		int					fluidWidth;
		int					fluidHeight;
		int					fluidNX;
		int					fluidNY;
		float				fluidInvWidth;
		float				fluidInvHeight;
		float				fluidVisc;
		float				fluidDT;
		float				fluidFadeSpeed;
		float				fluidInvNX;
		float				fluidInvNY;
	
		ofImage				tdScreenImage;
		ofTexture			tdTexture;
		unsigned char *		tdTexturePixels;
		int					tdWidth;
		int					tdHeight;
	
		MotionDetection		motionDetect;
	
		int					mouseX;
		int					mouseY;
		int					pmouseX;
		int					pmouseY;
};

#endif
