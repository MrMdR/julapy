#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "LiveAudioIn.h"
#include "OscPad.h"
#include "OpCirlce.h"
#include "OpBars.h"
#include "OpCheckers.h"
#include "OpScope.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
	
		void initAudioIn();
		void initOsc();
		void initOpScope();
		void initOpCheckers();
		void initOpCircle();
		void initOpBars();
		void initVideos();
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void audioReceived( float * input, int bufferSize, int nChannels );

		OscPad		oscPad;
	
		OpCirlce	opCirlce;
		int			opCirlceWidth;
		int			opCirlceHeight;
		int			opCirlceRes;
		float		opCirlceRot;
	
		OpBars		opBars;
		float		opBarsAudioAvgMin;
	
		OpCheckers	opCheckers;
		float		opCheckersSize;
	
		OpScope		opScope;
	
		LiveAudioIn		audioIn;
	
		ofVideoPlayer	video01;
		ofVideoPlayer	video02;
		ofVideoPlayer	video03;
		int				videoPlayIndex;
		bool			isVideo01Playing;
		bool			isVideo02Playing;
		bool			isVideo03Playing;
};

#endif
