#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	font.loadFont( "fonts/cooperBlack.ttf", 40 );
	
	screenGrabber.setup( "movie/" );
	screenGrabber.setPause( true );
	
	initClock();
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
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 20, 30 );
	
	screenGrabber.save();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == 's' )
		screenGrabber.togglePause();
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
//	clock.toggleClockMode();
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

