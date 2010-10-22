#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	
	assets = ofxAssets :: getInstance();
	
	assets->addImage( "ref_01",						"assets/ref/ref_01.jpg" );
	assets->addImage( "ref_02",						"assets/ref/ref_02.jpg" );
	assets->addImage( "ref_03",						"assets/ref/ref_03.jpg" );
	
	assets->addImage( "timeline_bg",				"assets/timeline_bg.png" );
	
	assets->addImage( "timeline_tab_rain",			"assets/timeline_tab_rain.png" );
	assets->addImage( "timeline_tab_rain_icon",		"assets/timeline_tab_rain_icon.png" );
	assets->addImage( "timeline_tab_rain_black",	"assets/timeline_tab_rain_black.png" );
	
	assets->addImage( "timeline_tab_temp",			"assets/timeline_tab_temp.png" );
	assets->addImage( "timeline_tab_temp_icon",		"assets/timeline_tab_temp_icon.png" );
	assets->addImage( "timeline_tab_temp_black",	"assets/timeline_tab_temp_black.png" );
	
	assets->addImage( "timeline_playBtn_rain_0",	"assets/timeline_playBtn_rain_0.png" );
	assets->addImage( "timeline_playBtn_rain_1",	"assets/timeline_playBtn_rain_1.png" );
	assets->addImage( "timeline_playBtn_temp_0",	"assets/timeline_playBtn_temp_0.png" );
	assets->addImage( "timeline_playBtn_temp_1",	"assets/timeline_playBtn_temp_1.png" );
	
	assets->addImage( "timeline_bar",				"assets/timeline_bar.png" );
	assets->addImage( "timeline_bar_scrubber",		"assets/timeline_bar_scrubber.png" );
	assets->addImage( "timeline_bar_marker_rain",	"assets/timeline_bar_marker_rain.png" );
	assets->addImage( "timeline_bar_marker_temp",	"assets/timeline_bar_marker_temp.png" );
	
	assets->addImage( "year_panel",					"assets/year_panel.png" );
	assets->addImage( "intro_panel",				"assets/intro_panel.png" );
	
	assets->addImage( "popup_bg",					"assets/popup_bg.png" );
	assets->addImage( "popup_icon_rain",			"assets/popup_icon_rain.png" );
	assets->addImage( "popup_icon_temp",			"assets/popup_icon_temp.png" );
	assets->addImage( "popup_play",					"assets/popup_play.png" );
	assets->addImage( "popup_shadow",				"assets/popup_shadow.png" );
	
	assets->addImage( "event_thumb_0",				"assets/event_thumbs/event_thumb_0.png" );
	assets->addImage( "event_thumb_1",				"assets/event_thumbs/event_thumb_1.png" );
	
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

