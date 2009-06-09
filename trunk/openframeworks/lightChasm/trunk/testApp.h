#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "LiveAudioIn.h"
#include "OscPad.h"
#include "OpCirlce.h"
#include "OpBars.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
	
		LiveAudioIn		audioIn;
	
		ofVideoPlayer	testVideo;
};

#endif
