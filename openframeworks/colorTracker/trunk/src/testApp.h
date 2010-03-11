#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"
#include "laserTracking.h"
#include "ofContourAnalysis.h"
#include "contourSimplify.h"

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
};

class testApp : public ofSimpleApp
{

public:

	void setup	();
	void update	();
	void draw	();
	
	void initGui	();

	void initContourAnalysis	();
	void computeContourAnalysis ();
	void drawContourAnalysis	();

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

	HSVData		*hsv;
	int			hsvTotal;
	int			hsvIndex;
	
	float	blur;
	float	threshold;
	
	//-- contour analysis.
	
	ofxCvContourFinder		contourFinder;
	contourSimplify			contourS;
	vector<ofxPoint2f>		*contourReg;
	vector<ofxPoint2f>		*contourSmooth;
	vector<ofxPoint2f>		*contourSimple;
	vector<ofxPoint2f>		*contourHull;
	CvBox2D32f				*box;
	float					*blobAngle;
	float					smoothPct;
	float					tolerance;
	int						runningBlobs;
	
	vector<ofxPoint4f>      geomLines;
	int						cfDetail;
	bool					bShowEllipse;
	bool					bShowAngle;
	bool					bShowLines;
	
protected:

	laserTracking LT;	
	
};

#endif
