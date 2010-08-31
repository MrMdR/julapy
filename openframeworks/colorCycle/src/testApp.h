#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ColorCycle.h"
#include "ColorSound.h"
#include "ColorConstants.h"
 
class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	
	void loadSounds			( string dirPath, vector<ofSoundPlayer*>& soundVector, bool loop = false );
	void loadNumOfSounds	( string dirPath, vector<ofSoundPlayer*>& soundVector, bool loop = false, int numOfSounds = CIRCLES_MAX );
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	int				frameRate;
	
	ColorCycle		cc;
	ColorSound		cs;
};

#endif
