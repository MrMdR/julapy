#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxVectorMath.h"

#define RIBBON_MAX_LENGTH  20

class testApp : public ofSimpleApp{

	public:

		void setup();
		void update();
		void draw();
		void drawRibbonFill();
		void drawRibbonMesh();
		void drawLetterOF();
	
		void drawRibbonType();
		void drawLetter( int letter );
		void drawLetterOnRibbon( int letter, float xOffset = 0, float yOffset = 0 );
		void drawLetterVertices();
		void clearLetterVertices();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		float rps[ RIBBON_MAX_LENGTH * 3 * 1 ];		// 3 points per ribbon location.
		float rvt[ RIBBON_MAX_LENGTH * 3 * 2 ];		// 3 points per vertex and 2 vertex per ribbon section.
		float rcl[ RIBBON_MAX_LENGTH * 4 * 2 ];		// 4 colour values per vertex
	
		ofTrueTypeFont font;
		int fontSize;
		int letter;
		vector <float*> polyVertices;
		int currentStartVertex;
};

#endif

