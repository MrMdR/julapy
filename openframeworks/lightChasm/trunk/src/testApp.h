#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "LiveAudioIn.h"
#include "DataNormaliser.h"
#include "OpCirlce.h"
#include "OpBars.h"
#include "OpCheckers.h"
#include "OpScope.h"

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
	
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void audioReceived( float * input, int bufferSize, int nChannels );

		ofxOscReceiver	oscReceiver;
		OscReceiver		oscRcvr;
	
		OpCirlce	opCirlce;
		int			opCirlceWidth;
		int			opCirlceHeight;
		int			opCirlceRes;
		float		opCirlceResScale;
		float		opCirlceRot;
		float		opCirlceRotScale;
		float		opCirlceColor;
	
		OpBars		opBars;
		float		opBarsAudioAvgMin;
		float		opBarsAudioAvgMinScale;
	
		OpCheckers	opCheckers;
		float		opCheckersSize;
		float		opCheckersSizeScale;
	
		OpScope		opScope;
	
		LiveAudioIn		audioIn;
		DataNormaliser	audioInAvgPower;
	
		ofVideoPlayer	*videos;
		bool			*videoPlayStates;
		int				videosTotal;
		int				videoIndex;
};

#endif
