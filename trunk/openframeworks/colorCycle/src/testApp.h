#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxDirList.h"
#include "ColorCycle.h"

class testApp : public ofBaseApp{

public:
	
	void setup();
	void update();
	void draw();
	
	void loadSounds	( string dirPath, vector<ofSoundPlayer*>& soundVector );
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	int				frameRate;
	
	ColorCycle		cc;
	
	ofxDirList				DIR;
	vector<ofSoundPlayer*>	spBackground;
	vector<ofSoundPlayer*>	spMeshDrag;
	vector<ofSoundPlayer*>	spPointAdd;
	vector<ofSoundPlayer*>	spPointRemove;
	vector<ofSoundPlayer*>	spPointCollide;
	vector<ofSoundPlayer*>	spPointShuffle;
	
};

#endif
