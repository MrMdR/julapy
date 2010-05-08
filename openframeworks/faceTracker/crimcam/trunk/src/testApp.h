#ifndef _TEST_APP
#define _TEST_APP


#define WIDTH				640
#define HEIGHT				480
#define SAMPLE_WIDTH		320
#define SAMPLE_HEIGHT		240

//#define	DEBUG_MODE

#include "ofMain.h"
#include "ofxCvHaarTracker.h"

class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();
	
		void drawFaceRect ( float x, float y, float w, float h );
		void drawBlackBar ( float x, float y, float w, float h );
		void drawCross ( float x, float y, float w, float h );
		void drawWantedSketch ( float x, float y, float w, float h );
		void drawBlur ( float x, float y, float w, float h );
		void drawSquares ( float x, float y, float w, float h );
		void drawCameraText ();
	
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		bool					bFullScreen;
	
		ofVideoGrabber			vidGrabber;
        
		ofxCvColorImage			colorLargeImage;
		ofxCvColorImage			colorSmallImage;
        ofxCvGrayscaleImage		grayLargeImage;
        ofxCvGrayscaleImage 	graySmallImage;
	
		ofxCvHaarFinder			haarFinder;
		ofxCvHaarTracker		haarTracker;

		float					sourceToSampleScale;
		float					sampleToSourceScale;
	
		ofxCvGrayscaleImage		blurImage;
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
