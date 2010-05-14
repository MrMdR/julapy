#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"
#include "ofxResizeUtil.h"

class testApp : public ofBaseApp
{

public:
	
	void setup	();
	void update	();
	void draw	();

	void initNoise		();
	void initOpenCv		();
	void initContours	();
	void initGui		();
	
	ofPoint getNoiseAtPoint		( const ofPoint& point );
	float	getNoiseAtPoint		( float x, float y );
	
	void updateNoiseImage		();
	void updateOpenCv			();
	int  updateContours			();
	void copyAndScaleBlobs		();
	
	void drawBorder					( const ofRectangle& rect );
	void drawNoiseImage				();
	void drawNoiseBand				();
	void drawNoiseBandSum			();
	void drawContoursSmall			();
	void drawContoursLarge			();
	void drawContourBoundingBoxes	( vector<ofxCvBlob>& blobs );
	void drawContourLines			( vector<ofxCvBlob>& blobs );
	void drawContourCurveLines		( vector<ofxCvBlob>& blobs );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	bool					bDebug;
	bool					bSmoothing;
	bool					bPause;

	ofRectangle				screenRect;
	ofRectangle				noiseRect;
	ofRectangle				debugRect;
	ofRectangle				largeRect;
	
	ofxCvGrayscaleImage		noiseImage;
	
	ofxCvGrayscaleImage		noiseBandSum;
	ofxCvGrayscaleImage*	noiseBands;
	float*					noiseBandCutoffs;
	float*					noiseBandSize;
	int						noiseBandsTotal;
	int						noiseBandIndex;
	
	ofxPerlin				noise;
	ofxVec3f				noiseLoc;
	ofxVec3f				noiseVel;
	ofxVec3f				noiseScl;
	float					noiseSclMaster;
	
	ofxCvContourFinder		contourFinder;
	vector<ofxCvBlob>		contourBlobsScaled;

};

#endif
