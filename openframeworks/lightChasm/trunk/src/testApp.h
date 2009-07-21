#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofEvents.h"
#include "LiveAudioIn.h"
#include "DataNormaliser.h"
#include "OpCirlce.h"
#include "OpBars.h"
#include "OpCheckers.h"
#include "OpScope.h"

//#include "OscPad.h"
#include "OscReceiver.h"
#include "TouchOscLayout01.h"

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
	
		void updateOsc();
		void updateVideo();
	
		void oscPadChangeEvent( int &x );
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void audioReceived( float * input, int bufferSize, int nChannels );

		ofxOscReceiver	oscReceiver;
//		OscPad			oscPad;
		float			slider01;
		float			slider02;
		float			slider03;
		float			slider04;
		float			slider05;
		float			slider06;
	
		OscReceiver		oscRcvr;
	
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
		DataNormaliser	audioInAvgPower;
	
		ofVideoPlayer	*videos;
		bool			*videoPlayStates;
		int				videosTotal;
		int				videoIndex;
};

#endif
