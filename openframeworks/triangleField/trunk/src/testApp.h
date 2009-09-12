#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxNoise.h"
#include "ofxTileSaver.h"
#include "ofxScreenGrabUtil.h"
#include "ColorPicker.h"
#include "ofxSimpleGuiToo.h"
#include "AudioFileSpectrum.h"
#include "ofxTrigger.h"

#include "TriangleField.h"
#include "TriangleFieldConfig.h"
#include "TriangleFieldConfig01.h"
#include "TriangleFieldConfig02.h"
#include "TriangleFieldConfig03.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void initRenderArea();
	void initFieldConfig();
	void initFields();
	void initDebug();
	void initBlendModes();
	void initGui();
	void initAudio();
	void initTriggers();

	void addBlendMode( GLuint srcBlend, GLuint dstBlend );
	
	void updateFields();
	void checkFieldConfigIndexChanged();
	
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
	ColorPicker			cpTest;
	ofxSimpleGuiToo		gui;
	
	bool				smoothing;
	
	int					blendModesTotal;
	int					blendModeIndex;
	GLuint				*blendModes;
	
	int					dbInc;
	int					frameCount;
	bool				showDebug;
	
	TriangleField		*fields;
	int					fieldsTotal;
	int					fieldIndex;
	
	TriangleFieldConfig	**fieldConfig;
	int					fieldConfigTotal;
	int					fieldConfigIndex;
	int					fieldConfigIndex2;
	
	ofxPerlin			noise;
	
	ofxTileSaver		tileSaver;
	ofxScreenGrabUtil	screenGrabUtil;
	
	AudioFileSpectrum	audio;
	
	ofxTrigger			trigger;
};

#endif
