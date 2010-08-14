#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );

	cc.setScreenSize( ofGetWidth(), ofGetHeight() );
	cc.setup();
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	cc.update();
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	cc.draw();
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{
	if( key == ' ' )
	{
		cc.togglePanel();
	}
}

void testApp :: keyReleased( int key )
{

}

void testApp :: mouseMoved( int x, int y )
{

}

void testApp :: mouseDragged( int x, int y, int button )
{
	cc.drag( x, y );
}

void testApp :: mousePressed( int x, int y, int button )
{
	cc.down( x, y );
}

void testApp :: mouseReleased( int x, int y, int button )
{
	cc.up( x, y );
}

void testApp :: windowResized( int w, int h )
{

}

