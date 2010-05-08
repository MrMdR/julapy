#include "ofMain.h"
#include "testApp.h"

//#define RIGHT_MONITOR

//========================================================================
int main( )
{
	ofSetupOpenGL( 1280, 768, OF_WINDOW );
	
	#ifdef RIGHT_MONITOR
	
		ofSetWindowPosition( 1440, 0 );
	
	#endif
	
	ofRunApp( new testApp() );
}
