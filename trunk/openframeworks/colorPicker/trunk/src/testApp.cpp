#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( 30 );

	Color c;
	c.r = (int)ofRandom( 0, 255 );
	c.g = (int)ofRandom( 0, 255 );
	c.b = (int)ofRandom( 0, 255 );
	c.a = 255;
	
	colorPicker0.setMode( COLOR_PICKER_MODE_MOUSE);
	colorPicker0.setColor( &c );
	
	colorPicker1.setMode( COLOR_PICKER_MODE_CIRLCE_ROTATION );
	colorPicker1.setCircularLowerBounds( 0.3 );
	
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
	
	drawColorPickers();
}

void testApp :: drawColorPickers()
{
	int py = 30;
	int px = 30;
	int sx = 10;

	colorPicker0.draw( px, py, 150, 150 );
	colorPicker1.draw( px += colorPicker0.getWidth() + sx, py, 200, 200 );
	colorPicker2.draw( px += colorPicker1.getWidth() + sx, py );
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

