#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );

	string xflPath;
	xflPath = "assets/LIBRARY/";
	
	assets = ofxAssets :: getInstance();
	
//	assets->addImage( "ref_01",						xflPath + "ref/ref_01.jpg" );
//	assets->addImage( "ref_02",						xflPath + "ref/ref_02.jpg" );
//	assets->addImage( "ref_03",						xflPath + "ref/ref_03.jpg" );
//	assets->addImage( "ref_04",						xflPath + "ref/ref_04.png" );
	
	assets->addImage( "timeline_bg",				xflPath + "assets/timeline_bg.png" );
	assets->addImage( "timeline_line_rain",			xflPath + "assets/timeline_line_rain.png" );
	assets->addImage( "timeline_line_temp",			xflPath + "assets/timeline_line_temp.png" );
	assets->addImage( "timeline_futureBtn",			xflPath + "assets/timeline_futureBtn.png" );
	
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
	
	assets->addImage( "map_elnino_icon_up",			xflPath + "assets/map_elnino_icon_up.png" );
	assets->addImage( "map_elnino_icon_over",		xflPath + "assets/map_elnino_icon_over.png" );
	assets->addImage( "elnino_copy",				xflPath + "elnino/elnino.png" );
	assets->addVideo( "elnino_video",				xflPath + "elnino/elnino.mov" );
	assets->addSound( "elnino_sound",				xflPath + "elnino/elnino.wav" );
	
	assets->addImage( "year_panel",					xflPath + "assets/year_panel.png" );
	assets->addImage( "intro_panel",				xflPath + "assets/intro_panel.png" );
	assets->addImage( "key_panel",					xflPath + "assets/key_panel.png" );
	
	assets->addImage( "popup_bg",					xflPath + "assets/popup_bg.png" );
	assets->addImage( "popup_icon_rain",			xflPath + "assets/popup_icon_rain.png" );
	assets->addImage( "popup_icon_temp",			xflPath + "assets/popup_icon_temp.png" );
	assets->addImage( "popup_play",					xflPath + "assets/popup_play.png" );
	assets->addImage( "popup_shadow",				xflPath + "assets/popup_shadow.png" );
	assets->addImage( "popup_target",				xflPath + "assets/popup_target.png" );
	
	assets->addImage( "event_thumb_0",				xflPath + "event_thumb/bushfire.png" );
	assets->addImage( "event_thumb_1",				xflPath + "event_thumb/cyclone.png" );
	assets->addImage( "event_thumb_2",				xflPath + "event_thumb/drought.png" );
	assets->addImage( "event_thumb_3",				xflPath + "event_thumb/flood.png" );
	assets->addImage( "event_thumb_4",				xflPath + "event_thumb/heatwave.png" );
	
	assets->addImage( "copy_2_2",					xflPath + "event_copy/2.2.png" );
	assets->addImage( "copy_2_3",					xflPath + "event_copy/2.3.png" );
	assets->addImage( "copy_2_4",					xflPath + "event_copy/2.4.png" );
	assets->addImage( "copy_3_1",					xflPath + "event_copy/3.1.png" );
	assets->addImage( "copy_3_2",					xflPath + "event_copy/3.2.png" );
	assets->addImage( "copy_3_3",					xflPath + "event_copy/3.3.png" );
	assets->addImage( "copy_3_4",					xflPath + "event_copy/3.4.png" );
	assets->addImage( "copy_4_1",					xflPath + "event_copy/4.1.png" );
	assets->addImage( "copy_4_2",					xflPath + "event_copy/4.2.png" );
	assets->addImage( "copy_4_3",					xflPath + "event_copy/4.3.png" );
	assets->addImage( "copy_4_4",					xflPath + "event_copy/4.4.png" );
	assets->addImage( "copy_4_5",					xflPath + "event_copy/4.5.png" );
	assets->addImage( "copy_5_1",					xflPath + "event_copy/5.1.png" );
	assets->addImage( "copy_5_2",					xflPath + "event_copy/5.2.png" );
	assets->addImage( "copy_5_3",					xflPath + "event_copy/5.3.png" );
	assets->addImage( "copy_5_4",					xflPath + "event_copy/5.4.png" );
	assets->addImage( "copy_5_5",					xflPath + "event_copy/5.5.png" );
	assets->addImage( "copy_6_1",					xflPath + "event_copy/6.1.png" );
	assets->addImage( "copy_6_2",					xflPath + "event_copy/6.2.png" );
	assets->addImage( "copy_6_3",					xflPath + "event_copy/6.3.png" );
	
	assets->addSound( "sound_2_2",					xflPath + "event_sound/2.2.wav" );
	assets->addSound( "sound_2_3",					xflPath + "event_sound/2.3.wav" );
	assets->addSound( "sound_2_4",					xflPath + "event_sound/2.4.wav" );
	assets->addSound( "sound_3_1",					xflPath + "event_sound/3.1.wav" );
	assets->addSound( "sound_3_2",					xflPath + "event_sound/3.2.wav" );
	assets->addSound( "sound_3_3",					xflPath + "event_sound/3.3.wav" );
	assets->addSound( "sound_3_4",					xflPath + "event_sound/3.4.wav" );
	assets->addSound( "sound_4_1",					xflPath + "event_sound/4.1.wav" );
	assets->addSound( "sound_4_2",					xflPath + "event_sound/4.2.wav" );
	assets->addSound( "sound_4_3",					xflPath + "event_sound/4.3.wav" );
	assets->addSound( "sound_4_4",					xflPath + "event_sound/4.4.wav" );
	assets->addSound( "sound_4_5",					xflPath + "event_sound/4.5.wav" );
	assets->addSound( "sound_5_1",					xflPath + "event_sound/5.1.wav" );
	assets->addSound( "sound_5_2",					xflPath + "event_sound/5.2.wav" );
	assets->addSound( "sound_5_3",					xflPath + "event_sound/5.3.wav" );
	assets->addSound( "sound_5_4",					xflPath + "event_sound/5.4.wav" );
	assets->addSound( "sound_5_5",					xflPath + "event_sound/5.5.wav" );
	assets->addSound( "sound_6_1",					xflPath + "event_sound/6.1.wav" );
	assets->addSound( "sound_6_2",					xflPath + "event_sound/6.2.wav" );
	assets->addSound( "sound_6_3",					xflPath + "event_sound/6.3.wav" );

	assets->addVideo( "video_2_2",					xflPath + "event_video/2.2.mov" );
	assets->addVideo( "video_2_3",					xflPath + "event_video/2.3.mov" );
	assets->addVideo( "video_2_4",					xflPath + "event_video/2.4.mov" );
	assets->addVideo( "video_3_1",					xflPath + "event_video/3.1.mov" );
	assets->addVideo( "video_3_2",					xflPath + "event_video/3.2.mov" );
	assets->addVideo( "video_3_3",					xflPath + "event_video/3.3.mov" );
	assets->addVideo( "video_3_4",					xflPath + "event_video/3.4.mov" );
	assets->addVideo( "video_4_1",					xflPath + "event_video/4.1.mov" );
	assets->addVideo( "video_4_2",					xflPath + "event_video/4.2.mov" );
	assets->addVideo( "video_4_3",					xflPath + "event_video/4.3.mov" );
	assets->addVideo( "video_4_4",					xflPath + "event_video/4.4.mov" );
	assets->addVideo( "video_4_5",					xflPath + "event_video/4.5.mov" );
	assets->addVideo( "video_5_1",					xflPath + "event_video/5.1.mov" );
	assets->addVideo( "video_5_2",					xflPath + "event_video/5.2.mov" );
	assets->addVideo( "video_5_3",					xflPath + "event_video/5.3.mov" );
	assets->addVideo( "video_5_4",					xflPath + "event_video/5.4.mov" );
	assets->addVideo( "video_5_5",					xflPath + "event_video/5.5.mov" );
	assets->addVideo( "video_6_1",					xflPath + "event_video/6.1.mov" );
	assets->addVideo( "video_6_2",					xflPath + "event_video/6.2.mov" );
	assets->addVideo( "video_6_3",					xflPath + "event_video/6.3.mov" );
	
	assets->addImage( "future_copy",				xflPath + "future/future.png" );
	assets->addSound( "future_sound",				xflPath + "future/future.wav" );
	
	assets->addImage( "event_close_icon",			xflPath + "assets/event_close_icon.png" );
	
	assets->addImage( "aus_cover_1280x720",			xflPath + "assets/aus_cover_1280x720.png" );
	assets->addImage( "aus_stroke_757x590",			xflPath + "assets/aus_stroke_757x590.png" );
	
	assets->addSound( "loop",						xflPath + "sounds/loop.mp3" );
	assets->addSound( "attractor",					xflPath + "sounds/attractor.wav" );
	assets->addSound( "instruct_0",					xflPath + "sounds/instruct_0.wav" );
	assets->addSound( "instruct_1",					xflPath + "sounds/instruct_1.wav" );
	
	ofSoundPlayer* loop = assets->getSound( "loop" );
	loop->setLoop( true );
	loop->setVolume( 0.2 );
	loop->play();
	
	ofSoundPlayer* attractor = assets->getSound( "attractor" );
	attractor->play();
	
	appView = new ApplicationView();
	appView->setup();

	bShowMouse = false;
	ofHideCursor();
	ofToggleFullscreen();
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
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
	
	if( key == 'm' )
	{
		bShowMouse = !bShowMouse;
		
		if( bShowMouse )
		{
			ofShowCursor();
		}
		else
		{
			ofHideCursor();
		}
	}
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

