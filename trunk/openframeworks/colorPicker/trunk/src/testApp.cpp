#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofEnableSmoothing();
	ofSetVerticalSync( true );
	ofSetBackgroundAuto( false );

	colorWheelWidth  = 300;
	colorWheelHeight = 300;

	int py = 30;
	int px = 30;
	int sx = 30;
	
	colorPicker0.init( px, py, colorWheelWidth, colorWheelHeight );
	colorPicker0.setMode( COLOR_PICKER_MODE_MOUSE);
	
	px += colorWheelWidth + sx;
	
	colorPicker1.init( px, py, colorWheelWidth, colorWheelHeight );
	colorPicker1.setMode( COLOR_PICKER_MODE_CIRLCE_ROTATION );
	colorPicker1.setCircularLowerBounds( 0.3 );
	
	px += colorWheelWidth + sx;
	
	colorPicker2.init( px, py, colorWheelWidth, colorWheelHeight );
	colorPicker2.setMode( COLOR_PICKER_MODE_RANDOM_WALK );
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	//
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	ofBackground( 200, 200, 200 );
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{

}

void testApp :: keyReleased( int key )
{

}

void testApp :: mouseMoved( int x, int y )
{

}

void testApp :: mouseDragged( int x, int y, int button )
{

}

void testApp :: mousePressed( int x, int y, int button )
{

}

void testApp :: mouseReleased( int x, int y, int button )
{

}

void testApp :: windowResized( int w, int h )
{

}

