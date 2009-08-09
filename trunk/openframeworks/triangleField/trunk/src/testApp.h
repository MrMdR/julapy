#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxTileSaver.h"
#include "ofxScreenGrabUtil.h"

class TriangleField
{
public :
	float				*sColor;
	float				*eColor;

	float				scale;
	float				cutoff;
	
	float				noiseX;
	float				noiseY;
	float				noiseZ;
	float				noiseXRes;
	float				noiseYRes;
	float				noiseZRes;
	float				noiseXVel;
	float				noiseYVel;
	float				noiseZVel;
};

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void initColor();
	void initFields();
	void initDebug();
	
	void addColor( int r, int g, int b, int a );
	
	void drawSquareNoise( TriangleField *field );
	void drawTraingleStatic();
	void drawTriangleNoise( TriangleField *field );
	void drawDebug();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	int					dbInc;
	int					frameCount;
	
	float				**colors;
	int					colorsMax;
	int					colorsTotal;
	
	TriangleField		*fields;
	int					fieldsTotal;
	int					fieldIndex;
	
	ofxPerlin			noise;
	
	ofxTileSaver		tileSaver;
	ofxScreenGrabUtil	screenGrabUtil;
};

#endif
