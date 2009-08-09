#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxVectorMath.h"
#include "trackball.h"
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


class testApp : public ofBaseApp
{

public:

	void setup();
	void update();
	void draw();

	void initAxis();
	void initEarth();
	void initLocations();
	void initRibbons();
	void initRays();
	void initLighting();
	void initDebug();
	
	void addLocation( float lat, float lon );
	
	void mouseSphereInit();
	void mouseSphereDown( int x, int y );
	void mouseSphereUp( int x, int y );
	void mouseSphereMove( int x, int y );
	void mouseSphereMoveResidual();
	
	void updateRibbons();
	void stepRibbonLocation( RibbonInfo *ribbon );
	void updateRibbonLocation( RibbonInfo *ribbon );
	
	void drawAxis();
	void drawSphere();
	void drawRibbons();
	void drawRays();
	void drawLatLonPlane();
	void drawDebug();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	int				dbInc;
	float			dbLat;
	float			dbLon;
	
	float			axisSize;
	
	ofImage			earthMap;
	ofTexture		earthMapTexture;
	GLUquadricObj	*quadratic;
	float			sphereSize;
	float			sphereOrientation[ 16 ];

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

#endif
