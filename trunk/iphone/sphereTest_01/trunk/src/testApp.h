#pragma once

#include "ofMain.h"
#include "ofxMultiTouch.h"
#include "ofxAccelerometer.h"

#include "ofxVectorMath.h"
#include "trackball.h"
#include "Sphere.h"
#include "SphereUtil.h"

#include "Ribbon.h"
#include "SphereRibbon.h"
#include "QuatRibbon.h"
#include "CurveHop.h"

class RibbonInfo
{
public :	
	Ribbon *ribbon;
	int locationIndex;
};

class Ray
{
public :
	ofxVec3f p;
};

class testApp : public ofSimpleApp, public ofxMultiTouchListener  {
	
public:
	void setup();
	void update();
	void draw();
	void exit();

	void initEarth();
	void initLocations();
	void initRibbons();
	void initRays();
	void initLighting();

	void addLocation( float lat, float lon );
	
	void initTrackball();
	void mouseTrackballDown( int x, int y );
	void mouseTrackballUp( int x, int y );
	void mouseTrackballMove( int x, int y );
	
	void mouseSphereInit();
	void mouseSphereDown( int x, int y );
	void mouseSphereUp( int x, int y );
	void mouseSphereMove( int x, int y );
	void mouseSphereMoveResidual();
	
	void updateRibbons();
	void stepRibbonLocation( RibbonInfo *ribbon );
	void updateRibbonLocation( RibbonInfo *ribbon );
	
	void drawSphere();
	void drawRibbons();
	void drawRays();
	void drawLines();
	void drawTriangle();
	void drawCube();
	
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
	ofImage		earthGlow;
	
	Sphere			sphere;
	
	RibbonInfo		*ribbons;
	int				ribbonsTotal;
	
	Ray				*rays;
	int				raysTotal;
	float			*rayTarget;
	ofImage			rayImage;
	ofImage			rayBaseImage;
	
	// trackball.
	GLfloat gTrackBallRotation[ 4 ];
	GLfloat gWorldRotation[ 4 ];
	bool	gTrackBall;
	
	// rotation.
	ofxVec2f	center;
	ofxVec2f	mouseCurr;
	ofxVec2f	mousePrev;
	ofxVec2f	mouseVel;
	bool		isMouseDown;
	
	// lighting.
	float	lightAmbient[ 4 ];
	float	lightDiffuse[ 4 ];
	float	matAmbient[ 4 ];
	float	matDiffuse[ 4 ];
	
	// locations.
	float	*locations;
	int		locationsTotal;
	int		locationsMax;
	int		locationIndex;
	
};

