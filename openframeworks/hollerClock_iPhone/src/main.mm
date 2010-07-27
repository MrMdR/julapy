#include "ofMain.h"
#include "clockApp.h"

int main()
{
//	ofSetupOpenGL( 1024, 768, OF_FULLSCREEN );			// <-------- setup the GL context
	ofSetupOpenGL( 480,  320, OF_FULLSCREEN );			// <-------- setup the GL context
	ofRunApp( new clockApp() );
}
