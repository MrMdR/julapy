#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "SphericalField.h"
#include "NoiseField.h"
#include "ofxTileSaver.h"
#include "RibbonType.h"

#define MAX_PARTICLES		1000
#define MAX_TRAIL_LENGTH	200

#define USE_VBO

class testApp : public ofBaseApp{

	public:
	
		void setup();
		void update();
		void draw();

		void initVBO();
		void initRibbonType();
		void mapColour( float x, float y, int *r, int *g, int *b );
		void drawPoint();
		void drawRibbonFill();	
		void drawRibbonFillVBO();
		void drawRibbonMesh();
		void drawRibbonType();
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		int stageWidth;
		int stageHeight;
		int stageCenterX;
		int stageCenterY;
	
		bool useAdditiveBlending;
		bool showRibbonType;
	
		NoiseField		noiseField;
		SphericalField	sphericalField;
	
		ofImage			colourMapImage;
	
		int		pTotal;
		int		posSpread;
		int		velSpread;
		int		trailIndex;
		int		trailCount;
		float	pos[ MAX_PARTICLES ][ 3 ];
		float	vel[ MAX_PARTICLES ][ 3 ];
		float	col[ MAX_PARTICLES ][ 4 ];
		float	trl[ MAX_PARTICLES ][ 3 * MAX_TRAIL_LENGTH ];		// trail position.
		float	tvd[ MAX_PARTICLES ][ 3 * MAX_TRAIL_LENGTH ];		// trail vertex direction.
		float	tvr[ MAX_PARTICLES ][ 3 * MAX_TRAIL_LENGTH * 2 ];	// trail vertexes.
		float	tcl[ MAX_PARTICLES ][ 4 * MAX_TRAIL_LENGTH * 2 ];	// trail colour.
		GLuint	vbo[ MAX_PARTICLES * 2 ];
	
		ofxVec3f	upAxis;
		float		upAxisRot;
		float		rotateY;
	
		ofxTileSaver	tileSaver;
	
		RibbonType	ribbonType;
};

#endif
