#include "testApp.h"

/////////////////////////////////////////////
//	SETUP.
/////////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	
	ct.init( 4 );
	ct.loadFromFile();
}

/////////////////////////////////////////////
//	UPDATE.
/////////////////////////////////////////////

void testApp::update()
{
	ofBackground( 100,100,100 );

	ct.update();
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
