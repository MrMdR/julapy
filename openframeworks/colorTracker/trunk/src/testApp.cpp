#include "testApp.h"

/////////////////////////////////////////////
//	SETUP.
/////////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofSetBackgroundAuto( true );
	
	ct.init( 3 );
	ct.loadFromFile();
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void testApp::update()
{
	ct.update();
	
	const ColorTrackerData* data;	// data to be used externally.
	data = ct.getTrackerData();
}

/////////////////////////////////////////////
//	DRAW.
/////////////////////////////////////////////

void testApp::draw()
{
	ct.draw();
}

/////////////////////////////////////////////
//	HANDLERS.
/////////////////////////////////////////////

void testApp::keyPressed(int key)
{
	ct.keyPressed( key );
	
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}

void testApp::mouseMoved(int x, int y )
{
	ct.mouseMoved( x, y );
}

void testApp::mouseDragged(int x, int y, int button)
{
	ct.mouseDragged( x, y, button );
}

void testApp::mousePressed(int x, int y, int button)
{
	ct.mousePressed( x, y, button );
}

void testApp::mouseReleased()
{
	ct.mouseReleased();
}
