#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "laserTracking.h"
#include "ofContourAnalysis.h"
#include "contourSimplify.h"

#include <fstream>

#define		MAX_NUM_CONTOURS_TO_FIND	20
#define		TOTAL_OBJECTS_TRACKED		3

struct HSVData
{
	float	hue, sat, val;
	float	hueRange, satRange, valRange;
	
	int		minHue, maxHue;
	int		minSat, maxSat;
	int		minVal, maxVal;
	
	ofxCvGrayscaleImage		img;
	unsigned char *         imgPixels;
	
	bool	bShowImg;
	bool	bShowContour;
};

struct ContourData
{
	int						nBlobs;

	vector<float>			blobArea;
	vector<ofRectangle>		blobBoundingRect;
	vector<ofPoint>			blobCentroid;
	
	CvBox2D32f				*box;
	float					*blobAngle;
	
	vector<ofxPoint4f>      geomLines;
	
	vector<ofxPoint2f>		*contourReg;
	vector<ofxPoint2f>		*contourSmooth;
	vector<ofxPoint2f>		*contourSimple;
	vector<ofxPoint2f>		*contourHull;
};

class testApp : public ofSimpleApp
{

public:

	void setup	();
	void update	();
	void draw	();
	
	void initGui				();
	void initContourAnalysis	();
	
	void computeContourAnalysis ( int i );
	void drawContourAnalysis	( int i );
	
	void writeToFile	( string filename = "config" );
	void loadFromFile	( string filename = "config" );

	void keyPressed		( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	();

	ofVideoGrabber			video;
	ofRectangle				videoRect;

	ofxCvColorImage		    colImg;
	ofxCvColorImage		    hsvImg;
	ofxCvGrayscaleImage		hueImg;
	ofxCvGrayscaleImage		satImg;
	ofxCvGrayscaleImage		valImg;

	float	blur;
	float	threshold;
	
	HSVData			*hsv;
	int				hsvTotal;
	int				hsvIndex;
	
	ContourData		*cdata;
	int				cdataTotal;
	
	ofxCvContourFinder		contourFinder;
	contourSimplify			contourS;

	float					smoothPct;
	float					tolerance;
	
	int						cfDetail;
	float					cfMinArea;
	bool					bShowBox;
	bool					bShowEllipse;
	bool					bShowAngle;
	bool					bShowLines;
	
protected:

	laserTracking LT;	
	
};

#endif
