#include "ofMain.h"
#include "testApp.h"

//#define	FULL_SCREEN

//========================================================================
int main( ){

	#ifdef FULL_SCREEN
	
		ofSetupOpenGL( 1280, 768, OF_FULLSCREEN );
	
	#else
	
		ofSetupOpenGL( 1280, 768, OF_WINDOW );
	
	#endif
	
	ofRunApp( new testApp() );
}
