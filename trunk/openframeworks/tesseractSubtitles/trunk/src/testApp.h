#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxMSAInteractiveObject.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "ofxTesseract.h"
#include "ofxScreenGrabUtil.h"
#include <fstream>

struct HSVData
{
	float	hue, sat, val;
	float	hueRange, satRange, valRange;
	
	int		minHue, maxHue;
	int		minSat, maxSat;
	int		minVal, maxVal;
};

class testApp : public ofBaseApp{

public:

	void setup		();
	void update		();
	void draw		();
	
	void initImage		();
	void initHSV		();
	void initMotionSub	();
	void initBlobs		();
	void initGui		();

	void checkHSV				();
	void checkMotionSub			();
	void checkBlobs				();
	void checkForCommonFill		( ofxCvGrayscaleImage& imageOut, ofxCvGrayscaleImage& image1, ofxCvGrayscaleImage& image2 );
	void checkImageChange		();
	
	void parseTesseractString	( string &str );
	
	void drawSourceImage		();
	void drawHSV				();
	void drawMotionImage		();
	void drawBlobs				();
	void drawFinal				();
	void drawBorder				( ofxCvImage& img, int color, int thickness );
	void drawROI				();
	void drawTesseractText		();
	
	void writeToFile	( string filename = "config" );
	void loadFromFile	( string filename = "config" );

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxScreenGrabUtil		screenGrab;
	
	bool					bCheckImage;
	bool					bImageChanged;
	
	ofTrueTypeFont			fontVerdana;
	ofTrueTypeFont			fontCooperBlack;

	ofRectangle				roi;
	
	ofxTesseract			tess;
	string					tessStr;
	
	ofVideoPlayer			image;
	ofRectangle				imageRect;
	ofxCvColorImage			imageColor;
	
	HSVData					hsv;
	ofxCvColorImage		    hsvSourceImg;
	ofxCvGrayscaleImage		hsvHueImg;
	ofxCvGrayscaleImage		hsvSatImg;
	ofxCvGrayscaleImage		hsvValImg;
	ofxCvGrayscaleImage		hsvProcessedImg;
	unsigned char *         hsvProcessedImgPixels;
	
	ofxCvGrayscaleImage		motionImg;
	
	ofxCvGrayscaleImage		blobImg;
	ofxCvContourFinder		contourFinder;
	
	ofxCvGrayscaleImage		finalImg;
	ofxCvGrayscaleImage		finalImgCopy;
	ofxCvGrayscaleImage		finalImgDiff;

	float					screenScale;
	bool					bIsPlaying;
	float					videoPosition;
	
	float					hsvDiffThreshold;
	int						hsvBlur;
	int						hsvBlurThreshold;
	
	ofRectangle				minBlobSize;
	ofRectangle				maxBlobSize;
};

#endif
