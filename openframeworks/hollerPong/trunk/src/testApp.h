#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ColorTracker.h"
#include "Pong.h"

class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();
	
	void drawVideo	();
	
	void initHaar			();
	void updateHaarFinder	( ofxCvColorImage& colImg );
	void drawHaar			();
	void drawHaarFace		();
	
	void updatePongPaddles	( float p1y, float p2y );
	void updatePongFlip		( float p1x, float p2x );
	void updatePongPause	( bool bPongPaused );
	void updatePongReset	( bool bPongReset );

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );

	//-- FACE FINDER.
	
	ofxCvHaarFinder			haarFinder;
	ofxCvGrayscaleImage		haarFinderImage;
	ofxCvColorImage			haarFaceImage;
	ofImage					haarFaceMask;
	ofTexture				haarFaceAlpha;
	ofRectangle				haarRect;
	
	bool					bHaarFaceFound;
	bool					bHaarFirstFaceFound;
	bool					bHaarUpdateFace;
	
	int						haarTimeoutCount;
	int						haarTimeoutTotal;
	
	
	//-- COLOR TRACKER.
	
	
	ColorTracker	ct;
	
	
	//-- PONG.
	
	Pong			pong;
	
	float			p1;
	float			p2;
	
	bool			bShowDebug;
	bool			bUseMouse;
	
	bool			bPongPaused0;
	bool			bPongPaused1;
};

#endif
