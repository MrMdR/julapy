#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	bDebug = true;
	
	font.loadFont( "fonts/cooperBlack.ttf", 40 );
	
	screenGrabber.setup( "movie/" );
	screenGrabber.setPause( true );
	
	initClock();
	initGui();
}
	
void testApp :: initClock ()
{
	box2d.init();
	box2d.createBounds();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.registerGrabbing();
	
	clock.setBox2d( &box2d );
	clock.setTimeFont( &font );
	clock.setup();
}

void testApp :: initGui ()
{
	gui.addTitle( "clock" );
	gui.addSlider( "forceCenterPull",	clock.forceCenterPull,	0, 100 );
	gui.addSlider( "forceCenterPush",	clock.forceCenterPush,	0, 100 );
	gui.addSlider( "rayBlobPad",		clock.rayBlobPad,		0, 100 );
	gui.addSlider( "rayBlobEase",		clock.rayBlobEase,		0, 1 );
	
	gui.setPage( 1 );
	
	if( bDebug )
		gui.show();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	int hrs;
	int min;
	int sec;
	
	if( screenGrabber.isRecording() )
	{
		int frameNum = ofGetFrameNum();
		
		int s = 00 + frameNum / frameRate;		// start time is add at front.
		int m = 48 + s / 60;					// start time is add at front.
		int h = 12 + m / 60;					// start time is add at front.
		
		hrs = h % 24;
		min = m % 60;
		sec = s % 60;
	}
	else
	{
		hrs = ofGetHours();
		min = ofGetMinutes();
		sec = ofGetSeconds();
	}
	
	clock.update( hrs, min, sec );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	int bg = 0;
	ofBackground( bg, bg, bg );
	
	clock.draw();
	
	ofSetColor( 0xFFFFFF );
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 15, ofGetHeight() - 15 );

	gui.draw();
	
	screenGrabber.save();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == 's' )
		screenGrabber.togglePause();
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
		
		if( bDebug )
		{
			gui.show();
		}
		else
		{
			gui.hide();
		}
	}
	
	if( key == 'm' )
	{
		clock.toggleClockMode();
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

}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

