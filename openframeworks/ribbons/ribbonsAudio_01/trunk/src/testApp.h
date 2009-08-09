#ifndef _TEST_APP
#define _TEST_APP

#define USE_VBO
//#define USE_LIVE_AUDIO

#include "ofMain.h"
#include "SphereBg.h"
#include "SphericalField.h"
#include "NoiseField.h"
#include "ofxTileSaver.h"
#include "ofxScreenGrabUtil.h"

#ifdef USE_LIVE_AUDIO
	#include "RibbonLiveAudio.h"
#else
	#include "RibbonFileAudio.h"
#endif

#define MAX_PARTICLES		1000
#define MAX_TRAIL_LENGTH	100

class testApp : public ofBaseApp{

	public:
	
		void setup();
		void update();
		void draw();

		void initVBO();
		void mapColour( float x, float y, int *r, int *g, int *b );
		void drawPoint();
		void drawRibbonFill();	
		void drawRibbonFillVBO();
		void drawRibbonMesh();
	
		void audioReceived(float * input, int bufferSize, int nChannels );
		void keyPressed(int key);
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
	
		NoiseField		noiseField;
		SphericalField	sphericalField;
	
		SphereBg		sphereBg;
		ofImage			colourMapImage;
	
		int		pTotal;
		int		posSpread;
		int		velSpread;
		int		trailIndex;
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
	
		ofxTileSaver		tileSaver;
		ofxScreenGrabUtil	screenGrabUtil;
	
#ifdef USE_LIVE_AUDIO
		RibbonLiveAudio		ribbonLiveAudio;
#else
		RibbonFileAudio		ribbonFileAudio;
#endif
	
};

#endif
