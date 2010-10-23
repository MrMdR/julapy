#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );

	string xflPath;
	xflPath = "assets/LIBRARY/";
	
	
	assets = ofxAssets :: getInstance();
	
	assets->addImage( "ref_01",						xflPath + "ref/ref_01.jpg" );
	assets->addImage( "ref_02",						xflPath + "ref/ref_02.jpg" );
	assets->addImage( "ref_03",						xflPath + "ref/ref_03.jpg" );
	
	assets->addImage( "timeline_bg",				xflPath + "assets/timeline_bg.png" );
	
	assets->addImage( "timeline_tab_rain",			xflPath + "assets/timeline_tab_rain.png" );
	assets->addImage( "timeline_tab_rain_icon",		xflPath + "assets/timeline_tab_rain_icon.png" );
	assets->addImage( "timeline_tab_rain_black",	xflPath + "assets/timeline_tab_rain_black.png" );
	
	assets->addImage( "timeline_tab_temp",			xflPath + "assets/timeline_tab_temp.png" );
	assets->addImage( "timeline_tab_temp_icon",		xflPath + "assets/timeline_tab_temp_icon.png" );
	assets->addImage( "timeline_tab_temp_black",	xflPath + "assets/timeline_tab_temp_black.png" );
	
	assets->addImage( "timeline_playBtn_rain_0",	xflPath + "assets/timeline_playBtn_rain_0.png" );
	assets->addImage( "timeline_playBtn_rain_1",	xflPath + "assets/timeline_playBtn_rain_1.png" );
	assets->addImage( "timeline_playBtn_temp_0",	xflPath + "assets/timeline_playBtn_temp_0.png" );
	assets->addImage( "timeline_playBtn_temp_1",	xflPath + "assets/timeline_playBtn_temp_1.png" );
	
	assets->addImage( "timeline_bar",				xflPath + "assets/timeline_bar.png" );
	assets->addImage( "timeline_bar_scrubber",		xflPath + "assets/timeline_bar_scrubber.png" );
	assets->addImage( "timeline_bar_marker_rain",	xflPath + "assets/timeline_bar_marker_rain.png" );
	assets->addImage( "timeline_bar_marker_temp",	xflPath + "assets/timeline_bar_marker_temp.png" );
	
	assets->addImage( "year_panel",					xflPath + "assets/year_panel.png" );
	assets->addImage( "intro_panel",				xflPath + "assets/intro_panel.png" );
	assets->addImage( "key_panel",					xflPath + "assets/key_panel.png" );
	
	assets->addImage( "popup_bg",					xflPath + "assets/popup_bg.png" );
	assets->addImage( "popup_icon_rain",			xflPath + "assets/popup_icon_rain.png" );
	assets->addImage( "popup_icon_temp",			xflPath + "assets/popup_icon_temp.png" );
	assets->addImage( "popup_play",					xflPath + "assets/popup_play.png" );
	assets->addImage( "popup_shadow",				xflPath + "assets/popup_shadow.png" );
	
	assets->addImage( "event_thumb_0",				xflPath + "event_thumbs/event_thumb_0.png" );
	assets->addImage( "event_thumb_1",				xflPath + "event_thumbs/event_thumb_1.png" );
	
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

