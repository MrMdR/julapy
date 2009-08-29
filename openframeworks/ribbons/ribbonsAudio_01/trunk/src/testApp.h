#ifndef _TEST_APP
#define _TEST_APP

//#define USE_LIVE_AUDIO

#include "ofMain.h"

#include "Ribbon.h"
#include "RibbonView.h"
#include "RibbonFill.h"
#include "RibbonFillView.h"
#include "RibbonMesh.h"
#include "RibbonMeshView.h"

#include "AudioAbstract.h"
#include "AudioFileSpectrum.h"
#include "AudioLiveSpectrum.h"
#include "RibbonAudio.h"

#include "ofxColorMap.h"
#include "SphereBg.h"
#include "SphericalField.h"
#include "NoiseField.h"

#include "ofxTileSaver.h"
#include "ofxScreenGrabUtil.h"

#define MAX_PARTICLES		2000
#define MAX_TRAIL_LENGTH	100

class testApp : public ofBaseApp{

	public:
	
		void setup();
		void update();
		void draw();

		void initRenderArea();
		void initApp();
		void initColorMap();
		void initShereBg();
		void initFields();
		void initAudio();

		void initRibbons();
		void updtRibbons();
		void drawRibbons();
		
		void mapColour( float x, float y, int *r, int *g, int *b );

		void drawDebug();
	
		void toggleFullScreen();
	
		void audioReceived(float * input, int bufferSize, int nChannels );
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		ofRectangle		renderArea;
		ofRectangle		renderAreaWindow;
		ofRectangle		renderAreaFullScreen;
	
		ofxTileSaver		tileSaver;
		ofxScreenGrabUtil	screenGrabUtil;
	
		bool debugMode;
		int  incDir;
	
		ofxColorMap		colorMap;
		SphereBg		sphereBg;
	
		NoiseField		noiseField;
		SphericalField	sphericalField;
	
		int		pTotal;
		int		posSpread;
		int		velSpread;

		int		vboGlitch[ MAX_TRAIL_LENGTH ];
	
		Ribbon		**ribbons;
		RibbonView	**ribbonViews;
	
		float		rotateY;
	
		AudioAbstract	*audio;
		RibbonAudio		ribbonAudio;
		
};

#endif
