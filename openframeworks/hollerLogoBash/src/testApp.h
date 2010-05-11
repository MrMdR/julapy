#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxResizeUtil.h"
#include "ofxTriangle.h"
#include "ofxBox2d.h"
#include "ofxBox2dCircleCustom.h"
#include "ofxScreenGrabUtil.h"
#include "OpticalField.h"
#include "ofxColor.h"
#include "ofxCirclePacker.h"

struct Shape
{
	int						parent;
	int						child;
	int						noPolys;
	vector<ofPoint>			polyPoints;
};

struct TriangleShape
{
	ofPoint		vertex[ 3 ];
	int			colorFill;
	int			colorBorder;
	float		fade;
	int			alpha;
	int			count;
	int			countKill;
};

class testApp : public ofBaseApp{

public:
	
	void setup			();
	void update			();
	void draw			();
	
	void initColors			();
	void initLogo			();
	void initCamera			();
	void initCv				();
	void initContours		();
	void initBox2d			();
	void initOpticalField	();
	void initCirclePacker	();
	
	void updateCamera			();
	void updateCv				( unsigned char *pixels );
	int  updateContours			();
	void addCirclesToBox2d		();
	void updateCirclePacker		();
	void updateCircles			();
	void killCircles			();
	
	void parseShapes				();
	void scaleShapes				();
	void clearShapes				();
	bool checkEmbeddedRectangles	( const ofRectangle& r1, const ofRectangle& r2 );
	void copyPolygonData			( const vector<ofPoint>& p1, vector<ofPoint>& p2 );
	void copyPolygonData			( const vector<ofxPoint2f>& p1, vector<ofPoint>& p2 );
	
	void drawDebug			();
	void drawBorder			( const ofRectangle& rect );
	void drawShapes			();
	void drawShape			( Shape& shape );
	void drawCirclePacker	();
	void drawCircles		();

	void keyPressed		(int key);
	void keyReleased	(int key);
	void mouseMoved		(int x, int y );
	void mouseDragged	(int x, int y, int button);
	void mousePressed	(int x, int y, int button);
	void mouseReleased	(int x, int y, int button);
	void windowResized	(int w, int h);
	
	ofxScreenGrabUtil		screenGrab;

	bool					bDebug;
	bool					bFullScreen;
	bool					bSmooth;
	int						frameRate;
	
	ofImage					logo;
	unsigned char*			logoPixles;
	ofImage					logoAlpha;
	ofImage					logoJpg;

	ofxCvGrayscaleImage		logoBW;
	unsigned char*			logoBWPixles;
	ofRectangle				logoCropRect;
	
	ofVideoGrabber			camera;
	ofVideoPlayer			video;
	ofRectangle				cameraRect;
	bool					cameraNewFrame;
	bool					bUseCamera;
	
	ofxCvColorImage			cameraColorImage;
	ofxCvGrayscaleImage		cameraGrayImage;
	ofxCvGrayscaleImage		cameraGrayPrevImage;
	ofxCvGrayscaleImage		cameraGrayDiffImage;
	
	ofxCvGrayscaleImage		logoSmall;
	ofxCvGrayscaleImage		logoSmallIntersect;
	
	ofxCvContourFinder		contourFinder;
	
	vector<Shape>			shapes;
	
	ofxTriangle             triangle;
	
	ofxBox2d				box2d;
	
	OpticalField			opticalField;
	ofRectangle				opticalFieldRect;
	
	vector<int>				colors;
	
	ofxCirclePacker			circlePacker;
	vector<ofxBox2dCircleCustom>	b2dCircles;
};

#endif
