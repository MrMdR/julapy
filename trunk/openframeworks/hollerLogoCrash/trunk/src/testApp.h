#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxBox2d.h"
#include "contourSimplify.h"
#include "ofxTriangle.h"
#include "ofxBox2d.h"
#include "ofxBox2dPolygonCustom.h"
#include "ofxBox2dCircleCustom.h"
#include "ofxScreenGrabUtil.h"

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
	vector<ofPoint>			*polyPoints;
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
	void copyPolygonData			( const vector<ofxPoint2f>& p1, vector<ofPoint>& p2 );
	void drawLogoShapes				();
	
	void updateTriangles			();
	void updateBox2dTriangles		();
	void drawTriangles				();
	void drawBox2dTriangles			();
	
	void addBox2dCircle				( ofPoint& pos, ofPoint& vel, float size = 10, float mass = 1.0, float bounce = 0.5, float friction = 0.3 );
	void drawBox2dCircles			();
	
	void initBox2d					();
	void addBody					( const vector<ofxTriangleData>& triangles );
	void makeComplexBody			( const vector<ofxTriangleData>& triangles, b2Body* body );
	void updateBodies				();
	void drawBodies					();

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	ofxScreenGrabUtil		screenGrab;
	
	ofImage					logo;
	unsigned char*			logoPixles;
	
	ofxCvGrayscaleImage		logoBW;
	unsigned char*			logoBWPixles;
	
	ContourData				cdata;
	
	vector<LogoShape>		shapes;
	
	ofxCvContourFinder		contourFinder;
	contourSimplify			contourS;
	
	ofxTriangle             triangle;
	
	ofxBox2d						box2d;
	vector<ofxBox2dPolygonCustom>	box2dTriangles;
	vector<ofxBox2dCircleCustom>	box2dCircles;
	
	vector<b2Body*>					bodies;
	vector<int>						shapeCnts;
	
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
