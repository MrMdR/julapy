#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//#define	FULL_SCREEN
//#define RIGHT_MONITOR

int main( )
{
	#ifdef FULL_SCREEN
	
		ofSetupOpenGL( 600, 480, OF_FULLSCREEN );
	
		#ifdef RIGHT_MONITOR
	
			ofSetWindowPosition( 1440, 0 );
	
		#endif
	
	#else
	
		ofSetupOpenGL( 1024, 768, OF_WINDOW );
	
	#endif

	ofRunApp( new testApp());
}
