#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "clockApp.h"
#include "filteringApp.h"
#include "contactApp.h"

//========================================================================
int main(  ){

    ofAppGlutWindow window;
	ofSetupOpenGL( &window, 1280,720, OF_WINDOW );
	
	ofRunApp( new clockApp() );
//	ofRunApp( new filteringApp() );
//	ofRunApp( new contactApp() );

}
