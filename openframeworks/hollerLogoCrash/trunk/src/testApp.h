#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"
#include "contourSimplify.h"

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

struct LogoShape
{
	int						parent;
	int						child;
	
	int						noPolys;
	vector<ofxPoint2f>		*polyPoints;
};

class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void initContourAnalysis		();
	void computeContourAnalysis		();
	void drawContourAnalysis		();
	
	void parseLogoShapes			();
	bool checkEmbeddedRectangles	( const ofRectangle& r1, const ofRectangle& r2 );
	void copyPolygonData			( const vector<ofxPoint2f>& p1, vector<ofxPoint2f>& p2 );

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	ofImage					logo;
	unsigned char*			logoPixles;
	
	ofxCvGrayscaleImage		logoBW;
	unsigned char*			logoBWPixles;
	
	ofxBox2d				box2d;
	ContourData				cdata;
	
	vector<LogoShape>		shapes;
	
	ofxCvContourFinder		contourFinder;
	contourSimplify			contourS;
	
	int						maxContoursToFind;
	
	float					smoothPct;
	float					tolerance;
	
	int						cfDetail;
	float					cfMinArea;
	
	bool					bShowImage;
	bool					bShowRect;
	bool					bShowBox;
	bool					bShowEllipse;
	bool					bShowAngle;
	bool					bShowLines;
};

#endif
