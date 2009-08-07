#ifndef _TEST_APP
#define _TEST_APP

//#define USE_VIDEO_SAVER

#include "ofMain.h"
#include "LiveAudioIn.h"
#include "DataNormaliser.h"

#include "OpCirlce.h"
#include "OpBars.h"
#include "OpCheckers.h"
#include "OpScope.h"
#include "OpParticleRain.h"

#include "OscReceiver.h"
#include "TouchOscLayout01.h"

#include "ofxScreenGrabUtil.h"

#ifdef USE_VIDEO_SAVER
	#include "ofQtVideoSaver.h"
#endif


class VideoObj
{
public :		
	ofVideoPlayer	video;
	float			duration;
	int				frames;
	float			startTime;
	float			startFrame;
	bool			playing;
	bool			oscPlaying;
};

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
		void initOpRain();
		void initVideos();
		void initVideoSaver();
	
		void updateOsc();
		void updateVideo();
	
		void drawVideos();
		void drawToVideoSaver();
		void drawDebug();
	
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
	
		OpParticleRain opRain;
		float		   opRainMakeRate;
		float		   opRainMakeRateScale;
	
		OpScope		opScope;
	
		LiveAudioIn		audioIn;
		DataNormaliser	audioInAvgPower;
	
		VideoObj	*videos;
		int			videosTotal;
		bool		videoPositionOverride;
		float		videoPositionOsc;
		float		videoPosition;
	
#ifdef USE_VIDEO_SAVER
		ofQtVideoSaver		videoSaver;
		ofImage				videoSaverImage;
		int					videoSaverWidth;
		int					videoSaverHeight;
		bool				videoSaverRecording;
		string				videoSaverPath;
#endif
	
		ofxScreenGrabUtil	screenGrabUtil;
};

#endif
