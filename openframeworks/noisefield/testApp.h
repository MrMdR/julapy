#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "NoiseField.h"
#include "ParticleSystem.h"
#include "OpticalField.h"

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
	
	NoiseField	*noiseField;
	int			noiseFieldWidth;
	int			noiseFieldHeight;
	int			noiseFieldRes;
	float		noiseFieldDX;
	float		noiseFieldDY;
	float		noiseFieldDZ;
	
	ParticleSystem	*ps;
	int				psWidth;
	int				psHeight;
	
	OpticalField	*opticalField;
	int				camWidth;
	int				camHeight;
};

#endif
