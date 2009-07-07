#include "ofMain.h"
#include "testApp.h"

#define	FULL_SCREEN
#define RIGHT_MONITOR

//========================================================================
int main( ){

	#ifdef FULL_SCREEN
	
		ofSetupOpenGL( 1280, 768, OF_FULLSCREEN );
	
		#ifdef RIGHT_MONITOR
		
			ofSetWindowPosition( 1440, 0 );
	
		#endif
	
	#else
	
		ofSetupOpenGL( 1280, 768, OF_WINDOW );
	
	#endif
	
	ofRunApp( new testApp() );
}
