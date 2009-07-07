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
	
		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();

		ofVideoGrabber			vidGrabber;
        
		ofxCvColorImage			colorLargeImage;
		ofxCvColorImage			colorSmallImage;
        ofxCvGrayscaleImage		grayLargeImage;
        ofxCvGrayscaleImage 	graySmallImage;
	
		ofxCvHaarFinder			haarFinder;
		ofxCvHaarTracker		haarTracker;

		float					sourceToSampleScale;
		float					sampleToSourceScale;
};

#endif
