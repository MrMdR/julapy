#ifndef _TEST_APP
#define _TEST_APP


#define WIDTH				640
#define HEIGHT				480
#define SAMPLE_WIDTH		320
#define SAMPLE_HEIGHT		240

//#define	DEBUG_MODE

#include "ofMain.h"
#include "ofxCvHaarTracker.h"
#include "Animation.h"

class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();
	
		void loadCarlaWin();
		void loadDaftPunkWin();
		void loadMarianneWin();
	
		void drawCarlaWin( float x, float y, float w, float h );
		void drawDaftPunkWin( float x, float y, float w, float h );
		void drawMarianneWin( float x, float y, float w, float h );
	
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
	
		Animation		carla_win;
		Animation		daftpunk_win;
		Animation		marianne_win;
};

#endif
