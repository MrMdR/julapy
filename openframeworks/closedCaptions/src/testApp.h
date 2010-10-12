#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxSrt.h"
#include "ofxScreenGrabUtil.h"
#include "SpeachBubble.h"
#include "ofxOpenCv.h"
#include "ofxCvHaarFinder.h"
#include "ofxCvHaarTracker.h"
#include "AudioFileSpectrum.h"
#include "AudioLiveSpectrum.h"

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();

	void updateHaarFinder		();
	void drawHaarTracker		();
	
	void drawBorder				( int w, int h );
	void drawProgressBar		();
	
	void addSpeachBubble		( string bubbleText );
	void playOutSpeachBubbles	();
	void updateSpeachBubbles	();
	void drawSpeachBubbles		();
	void drawSpeachBubble		( SpeachBubble& bubble );
	
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	bool					bIsPlaying;
	
	ofxScreenGrabUtil		screenGrab;
	
	ofxSrt					srt;
	ofVideoPlayer			video;
	ofRectangle				videoRect;
	bool					bVideoPaused;
	float					videoSmlScale;
	float					videoProgress;
	
	ofxCvColorImage			videoCol;
	ofxCvGrayscaleImage		videoImg;
	ofxCvGrayscaleImage		videoSml;
	
	ofSoundPlayer			sound;
	float					soundTotalFrames;
	
	string					subtitleStr;
	ofTrueTypeFont			fontCooperBlack;
	ofTrueTypeFont			fontBubble;
	vector<SpeachBubble>	bubbles;
	
	ofxCvHaarFinder			haarFinder;
	ofxCvHaarTracker		haarTracker;
	ofRectangle				haarRect;
	ofRectangle				haarRectScaled;
	bool					bHaarFaceFound;
	
	AudioFileSpectrum		audio;
//	AudioLiveSpectrum		audio;
	float					audioPeak;
	float					*audioData;
	
};

#endif
