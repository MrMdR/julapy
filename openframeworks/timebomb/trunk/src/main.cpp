
#include "testApp.h"

//#define DEBUG						// SELECT ONLY ONE OF THESE
//#define FULLSCREEN					// SELECT ONLY ONE OF THESE
//#define FULLSCREEN_LEFT			// SELECT ONLY ONE OF THESE
//#define FULLSCREEN_RIGHT			// SELECT ONLY ONE OF THESE
#define RECORD					// SELECT ONLY ONE OF THESE

testApp *myApp;
int main( )
{

#ifdef DEBUG
	ofSetupOpenGL( 1360, 800, OF_WINDOW );
#endif
	
#ifdef FULLSCREEN
	ofSetupOpenGL( 1280, 800, OF_FULLSCREEN );
#endif
	
#ifdef FULLSCREEN_LEFT
	ofSetupOpenGL( 1280, 800, OF_FULLSCREEN );
	ofSetWindowPosition( -1920, 0 );
#endif
	
#ifdef FULLSCREEN_RIGHT
	ofSetupOpenGL( 1280, 800, OF_FULLSCREEN );
	ofSetWindowPosition( 1920, 0 );
#endif
	
#ifdef RECORD
	ofSetupOpenGL( 1280, 720, OF_WINDOW );
#endif
	
	myApp = new testApp;
	ofRunApp( myApp );
}
