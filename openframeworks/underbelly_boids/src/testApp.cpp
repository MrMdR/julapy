#include "testApp.h"

///////////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////////

void testApp :: setup ()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	
	bDebug			= true;
	
	initBoids();
	initGui();
}

void testApp :: initBoids ()
{
	flock.init();
}

void testApp :: initGui ()
{
	gui.addTitle( "boids" );
	gui.addSlider( "boidsNum ",				flock.boidsNumRevised,		0, 500   );
	gui.addSlider( "boidSeperationWeight ", flock.boidSeperationWeight,	0, 10.0  );
	gui.addSlider( "boidAlignmentWeight ",	flock.boidAlignmentWeight,	0, 10.0  );
	gui.addSlider( "boidCohesionWeight ",	flock.boidCohesionWeight,	0, 10.0  );
	gui.addSlider( "boidSeparationDist ",	flock.boidSeparationDist,	0, 500.0 );
	gui.addSlider( "boidPerception ",		flock.boidPerception,		0, 500.0 );
	gui.addSlider( "boidMaxSpeed ",			flock.boidMaxSpeed,			0, 100.0 );
	gui.addSlider( "boidMaxForce ",			flock.boidMaxForce,			0, 10.0  );
	gui.addSlider( "mouseReach ",			flock.mouseReach,			0, 200.0 );
	gui.addSlider( "mouseForce ",			flock.mouseForce,			-20.0, 0 );
	
	gui.addPage();
	gui.addTitle( "contour analysis" );
	
//	gui.loadFromXML();
	
	gui.show();
	gui.setPage( 1 );
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp :: update()
{
	updateBoids();
}

void testApp :: updateBoids ()
{
	flock.update();
}

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp :: draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	drawBoids();
	
	if( bDebug )
		return;
	
	gui.draw();
}

void testApp :: drawBoids ()
{
	flock.draw();
}

///////////////////////////////////////////////////
//	EVENTS.
///////////////////////////////////////////////////

void testApp :: keyPressed(int key)
{
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{
	flock.addMouse( x, y );
}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{
	flock.addFood( x, y );
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

