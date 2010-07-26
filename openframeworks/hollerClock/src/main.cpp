#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "clockApp.h"
#include "filteringApp.h"
#include "contactApp.h"
#include "jointApp.h"

//========================================================================
int main(  ){

    ofAppGlutWindow window;
	ofSetupOpenGL( &window, 1280, 720, OF_WINDOW );		// HD.
//	ofSetupOpenGL( &window, 1024, 768, OF_WINDOW );		// iPad.
//	ofSetupOpenGL( &window, 480,  320, OF_WINDOW );		// iPhone.
	
	ofRunApp( new clockApp() );
//	ofRunApp( new filteringApp() );
//	ofRunApp( new contactApp() );
//	ofRunApp( new jointApp() );

}
