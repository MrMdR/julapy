#include "testApp.h"

int main( )
{
    ofAppGlutWindow window;
	ofSetupOpenGL( &window, 1280, 720, OF_WINDOW );
	ofSetWindowPosition( 0, 0 );

	ofRunApp( new testApp() );
}
