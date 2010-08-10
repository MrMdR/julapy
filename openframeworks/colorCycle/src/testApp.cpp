#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );

	colorPicker0.setColorRadius( 1.0 );
	colorPicker1.setColorRadius( 1.0 );
	
	colorPicker0.setColorAngle( 0 );
	colorPicker1.setColorAngle( 1 / 3.0 );
	
	colorScale = 1.0;
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	if( bMouseDown )
	{
		float velx = mouseX - mousePos.x;
		float vely = mouseY - mousePos.y;
		
		mouseVel.x += ( velx - mouseVel.x ) * 0.1;
		mouseVel.y += ( vely - mouseVel.y ) * 0.1;
		
		mousePos.x = mouseX;
		mousePos.y = mouseY;
	}
	else
	{
		float ease = 0.98;
		mouseVel.x *= ease;
		mouseVel.y *= ease;
	}
	
	float ang;
	ang = mouseVel.y / 10000.0;
	ang += frameRate * 0.00001;
	
	colorPicker0.addToColorAngle( ang );
	colorPicker1.addToColorAngle( ang );

	colorScale += ( mouseVel.x / 10000.0 );
	colorScale = MAX( MIN( colorScale, 1.0 ), 0.5 );
	
	colorPicker0.setColorScale( colorScale );
	colorPicker1.setColorScale( colorScale );
	
	rect.setCornerColor( colorPicker0.getColor(), 0 );
	rect.setCornerColor( colorPicker0.getColor(), 1 );
	rect.setCornerColor( colorPicker1.getColor(), 2 );
	rect.setCornerColor( colorPicker1.getColor(), 3 );
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	rect.draw();
	
	int py = 30;
	int px = 30;
	int sx = 10;
	
	colorPicker0.draw( px, py, 200, 200 );
	colorPicker1.draw( px += colorPicker0.getWidth() + sx, py, 200, 200 );
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{
	if( key == ' ' )
	{
		colorPicker0.toggleShow();
		colorPicker1.toggleShow();
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
	//
}

void testApp :: mousePressed( int x, int y, int button )
{
	bMouseDown = true;
	
	mousePos.x = x;
	mousePos.y = y;
}

void testApp :: mouseReleased( int x, int y, int button )
{
	bMouseDown = false;
}

void testApp :: windowResized( int w, int h )
{

}

