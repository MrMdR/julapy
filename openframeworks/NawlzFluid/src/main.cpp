#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "NawlzFluidApp.h"
#include "NawlzFishBowlApp.h"
#include "NawlzBlahApp.h"
#include "NawlzSandApp.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL( &window, 1024, 768, OF_WINDOW );			// <-------- setup the GL context

//	ofRunApp( new NawlzFluidApp() );
	ofRunApp( new NawlzFishBowlApp() );
//	ofRunApp( new NawlzBlahApp() );
//	ofRunApp( new NawlzSandApp() );
}
