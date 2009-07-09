#pragma once

#include "ofMain.h"
#include "ofxMultiTouch.h"
#include "ofxAccelerometer.h"
#include "trackball.h"
#include "Sphere.h"

class testApp : public ofSimpleApp, public ofxMultiTouchListener  {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void initLighting();
	
	void initTrackball();
	void mouseTrackballDown( int x, int y );
	void mouseTrackballUp( int x, int y );
	void mouseTrackballMove( int x, int y );
	
	void drawTriangle();
	void drawCube();
	void drawLines();
	
	void keyPressed(int key) {}
	void keyReleased(int key)  {}
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void mouseReleased(int x, int y, int button );
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	
	ofImage		earthMap;
	ofTexture	earthMapTexture;
	
	Sphere sphere;
	
	// trackball.
	GLfloat gTrackBallRotation[ 4 ];
	GLfloat gWorldRotation[ 4 ];
	bool	gTrackBall;

	// lighting.
	float	lightAmbient[ 4 ];
	float	lightDiffuse[ 4 ];
	float	matAmbient[ 4 ];
	float	matDiffuse[ 4 ];	
};

