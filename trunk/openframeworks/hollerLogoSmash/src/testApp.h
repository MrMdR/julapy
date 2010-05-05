#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"
#include "ofxResizeUtil.h"
#include "ofxTriangle.h"
#include "ofxBox2d.h"
#include "ofxBox2dPolygonCustom.h"
#include "ofxBox2dCircleCustom.h"
#include "ofxScreenGrabUtil.h"

struct Shape
{
	int						parent;
	int						child;
	int						noPolys;
	vector<ofPoint>			polyPoints;
};

class testApp : public ofBaseApp{

public:
	
	void setup			();
	void update			();
	void draw			();
	
	void initLogo		();
	void initCamera		();
	void initCv			();
	void initContours	();
	void initBox2d		();
	
	void updateCamera			();
	void updateCv				( unsigned char *pixels );
	void updateContours			();
	void updateTriangles		();
	void addTrianglesToBox2d	();
	
	void parseShapes				();
	void clearShapes				();
	bool checkEmbeddedRectangles	( const ofRectangle& r1, const ofRectangle& r2 );
	void copyPolygonData			( const vector<ofPoint>& p1, vector<ofPoint>& p2 );
	void copyPolygonData			( const vector<ofxPoint2f>& p1, vector<ofPoint>& p2 );
	
	void drawBorder		( const ofRectangle& rect );
	
	void drawShapes		();
	void drawShape		( Shape& shape );
	void drawTriangles	();

	void keyPressed		(int key);
	void keyReleased	(int key);
	void mouseMoved		(int x, int y );
	void mouseDragged	(int x, int y, int button);
	void mousePressed	(int x, int y, int button);
	void mouseReleased	(int x, int y, int button);
	void windowResized	(int w, int h);
	
	ofxScreenGrabUtil		screenGrab;
		
	ofImage					logo;
	unsigned char*			logoPixles;

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
	ofxCvGrayscaleImage		logoLargeIntersectCrop;
	ofxCvGrayscaleImage		logoLargeIntersect;
	
	ofxCvContourFinder		contourFinder;
	
	vector<Shape>			shapes;
	
	ofxTriangle             triangle;
	
	ofxBox2d				box2d;
	ofxBox2dPolygonCustom	floor;
	
	vector<ofxBox2dPolygonCustom>	triangles;
};

#endif
