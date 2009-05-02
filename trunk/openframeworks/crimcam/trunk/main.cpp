#include "ofMain.h"
#include "testApp.h"

//#define	_DEBUG_MODE

//========================================================================
int main( ){

	#ifdef _DEBUG_MODE
		ofSetupOpenGL( 1280, 768, OF_WINDOW );			// <-------- setup the GL context
	#else
		ofSetupOpenGL( 1280, 768, OF_FULLSCREEN );
	#endif
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	ofRunApp(new testApp());
	
}
