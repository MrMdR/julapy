#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	assets = ofxAssets :: getInstance();
	assets->addImage( "timeline_tab_rain",		"assets/timeline_tab_rain.png" );
	assets->addImage( "timeline_tab_temp",		"assets/timeline_tab_temp.png" );
	assets->addImage( "timeline_tab_rain_icon", "assets/timeline_tab_rain_icon.png" );
	assets->addImage( "timeline_tab_temp_icon", "assets/timeline_tab_temp_icon.png" );
	assets->addImage( "timeline_tab_black",		"assets/timeline_tab_black.png" );
	
	appView = new ApplicationView();
	appView->setup();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	//
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	//
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{

}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{

}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{
	//
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

