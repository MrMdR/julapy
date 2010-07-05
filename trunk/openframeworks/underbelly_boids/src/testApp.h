#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "Boid.h"
#include "ofxSimpleGuiToo.h"

class testApp : public ofBaseApp
{

public :
	
	void setup			();
	void update			();
	void draw			();
	
	void initBoids		();
	void updateBoids	();
	void drawBoids		();
	
	void initGui		();

	void keyPressed		(int key);
	void keyReleased	(int key);
	void mouseMoved		(int x, int y );
	void mouseDragged	(int x, int y, int button);
	void mousePressed	(int x, int y, int button);
	void mouseReleased	(int x, int y, int button);
	void windowResized	(int w, int h);
	
	bool				bDebug;
	
	int					boidsNum;
	vector<Boid>		boids;
	
	float				boidSeperationWeight;
	float				boidAlignmentWeight;
	float				boidCohesionWeight;
	
	float				boidSeparationDist;
	float				boidNeighbourDist;
	float				boidMaxSpeed;
	float				boidMaxForce;

};

#endif
