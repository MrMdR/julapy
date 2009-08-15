#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main( )
{
//	ofSetupOpenGL( 1280, 720, OF_FULLSCREEN );
	ofSetupOpenGL( 1280, 720, OF_WINDOW );
//	ofSetWindowPosition( 200, 0 );
	
	ofRunApp( new testApp() );
}
