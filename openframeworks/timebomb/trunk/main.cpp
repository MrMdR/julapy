
#include "testApp.h"

testApp *myApp;
int main( )
{
	ofSetupOpenGL( 1280, 720, OF_WINDOW );			// <-------- setup the GL context
//	ofSetupOpenGL( 1280, 720, OF_FULLSCREEN );
//	ofSetWindowPosition( 1920, 0 );
	myApp = new testApp;
	ofRunApp( myApp );
}
