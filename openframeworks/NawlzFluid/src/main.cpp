#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "NawlzEffects.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL( &window, 1024, 768, OF_WINDOW );			// <-------- setup the GL context
    ofRunApp( new NawlzEffects() );
}
