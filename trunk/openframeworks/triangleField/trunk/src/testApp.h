#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxTileSaver.h"
#include "ofxScreenGrabUtil.h"
#include "ColorPicker.h"
#include "ofxSimpleGuiToo.h"

class TriangleField
{
public :
	float				*sColor;
	float				*eColor;
	float				*bsColor;
	float				*beColor;

	float				scale;
	int					scaleInc;
	float				cutoff;
	
	bool				drawOutline;
	
	float				noiseX;
	float				noiseY;
	float				noiseZ;
	
	float				noiseXInit;
	float				noiseYInit;
	float				noiseZInit;
	
	float				noiseXRes;
	float				noiseYRes;
	float				noiseZRes;
	
	float				noiseXVel;
	float				noiseYVel;
	float				noiseZVel;
	
	float				noiseXScl;
	float				noiseYScl;
};

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void initRenderArea();
	void initColor();
	void initFields();
	void initDebug();
	void initGui();
	
	void addColor( int r, int g, int b, int a );
	
	void updateFieldColors();
	
	void drawSquareNoise( TriangleField *field );
	void drawTraingleStatic();
	void drawTriangleNoise( TriangleField *field );
	void drawDebug();
	
	void toggleFullScreen();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofRectangle			renderArea;
	ofRectangle			renderAreaWindow;
	ofRectangle			renderAreaFullScreen;
	
	ColorPicker			*colorPickers;
	ofxSimpleGuiToo		gui;
	
	int					dbInc;
	int					frameCount;
	bool				showDebug;
	
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
