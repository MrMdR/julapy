#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxMSAInteractiveObject.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "ofxTesseract.h"

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();
	
	void initGui ();
	void parseTesseractString ( string &str );
	
	void findTextinStaticImage();
	void findTextInCameraImage();
	
	void drawSourceImage		();
	void drawProcessedImage		();
	void drawProcessedCropImage	();
	void drawVideoSource		();
	void drawVideoProcessed		();
	void drawROI				();
	void drawTesseractText		();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofTrueTypeFont			fontVerdana;
	ofTrueTypeFont			fontCooperBlack;

	int						threshold_1;
	int						blur_1;
	int						threshold_2;
	bool					bShowFullImage;
	
	ofRectangle				roi;
	
	ofImage					image;
	ofxCvColorImage			imageColor;
	ofxCvGrayscaleImage		imageGray;
	ofxCvGrayscaleImage		imageGrayCrop;
	
	ofVideoGrabber			camera;
	bool					bCheckCameraImage;
	ofxCvColorImage			cameraColorImage;
	ofxCvGrayscaleImage		cameraGrayImage;
	
	ofxTesseract			tess;
	string					tessStr;
	
};

#endif
