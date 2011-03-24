#include "ofMain.h"
#include "ofAppGlutWindow.h"

#include "NawlzFluidApp.h"
#include "NawlzFishBowlApp.h"
#include "NawlzBlahApp.h"
#include "NawlzSandApp.h"
#include "NawlzJackInBoxApp.h"
#include "NawlzInfodelApp.h"
#include "NawlzQuestionsApp.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL( &window, 1024, 768, OF_WINDOW );			// <-------- setup the GL context

    switch( 5 )
    {
        case 0 : ofRunApp( new NawlzFluidApp()      );  break;
        case 1 : ofRunApp( new NawlzFishBowlApp()   );  break;
        case 2 : ofRunApp( new NawlzBlahApp()       );  break;
        case 3 : ofRunApp( new NawlzSandApp()       );  break;
        case 4 : ofRunApp( new NawlzJackInBoxApp()  );  break;
        case 5 : ofRunApp( new NawlzInfodelApp()    );  break;
        case 6 : ofRunApp( new NawlzQuestionsApp()  );  break;
    }
}
