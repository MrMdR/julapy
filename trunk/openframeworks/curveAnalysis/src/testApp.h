#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxContourUtil.h"
#include "ofxCvContourSimplify.h"
 
class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void drawCurve			( const vector<ofPoint>& points );
	void drawFittedCurve	( const vector<ofPoint>& points );

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	ofxContourUtil			cu;
	ofxCvContourSimplify	cs;
	
	vector<ofPoint>		curve;
	vector<ofPoint>		curveSmooth;
	vector<ofPoint>		curveSimplify;
	vector<ofPoint>		curveCvSimplify;
	
	float				scale1;
	float				scale2;
	float				scale3;
	
	bool				bMouseDown;
};

#endif
