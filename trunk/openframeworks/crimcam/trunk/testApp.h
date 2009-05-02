#ifndef _TEST_APP
#define _TEST_APP


#define OF_ADDON_USING_OFXOPENCV

#define WIDTH				640
#define HEIGHT				480
#define SAMPLE_WIDTH		320
#define SAMPLE_HEIGHT		240


#include "ofMain.h"

#define _USE_LIVE_VIDEO
//#define	_DEBUG_MODE

#include "faceTracker.h"

class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();
	
		void drawFaceRect ( float x, float y, float w, float h, float cx, float cy );
		void drawBlackBar ( float x, float y, float w, float h, float cx, float cy );
		void drawCross ( float x, float y, float w, float h, float cx, float cy );
		void drawWantedSketch ( float x, float y, float w, float h, float cx, float cy );
		void drawBlur ( float x, float y, float w, float h, float cx, float cy );
		void drawSquares ( float x, float y, float w, float h, float cx, float cy );
		void drawCameraText ();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif
        
		ofxCvColorImage			colorLargeImage;
		ofxCvColorImage			colorSmallImage;
        ofxCvGrayscaleImage		displayImage;
        ofxCvGrayscaleImage 	sampleImage;
		ofxCvGrayscaleImage		blurImage;
	
		FaceTracker				faceTracker;

		float					sourceToSampleScale;
		float					sampleToSourceScale;
	
		ofImage					cross;
		ofImage					wantedSketch;
		ofImage					interlace;
		int						interlaceCounter;
		int						interlaceY;
	
		unsigned char			*blurPixels;
		ofTexture				blurTexture;
		unsigned char			*squarePixels;
		ofTexture				squareTexture;
		int						crimModeIndex;
		int						crimModeTotal;
	
		ofTrueTypeFont			krungthepSmall;
		ofTrueTypeFont			krungthepLarge;
		int						milliSecondCounter;
		
};

#endif
