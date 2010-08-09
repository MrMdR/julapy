#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 60 );
	ofBackground( 30, 30, 30 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	sc.init();
	
	stage = ofxStage::stage();		// reference to stage.
	
	ofxSprite* magenta;
	magenta			= stage->addChild( new Magenta() );
	magenta->name	= "magenta";
	
	ofxSprite* yellow;
	yellow			= stage->addChild( new Yellow() );
	yellow->name	= "yellow";

	ofxSprite* red;
	red				= stage->addChild( new Red() );
	red->name		= "red";
	
	for( int i=0; i<10; i++ )
	{
		yellow->addChild( new Cyan() );
	}
	
	for( int i=0; i<10; i++ )
	{
		magenta->addChild( new Cyan() );
	}
	
	delete magenta;
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	ofxSprite* yellow;
	yellow = stage->getChildByName( "yellow" );
	
	ofPoint global;
	
	if( yellow != NULL )
	{
		global = yellow->getChildAt( 0 )->localToGlobal( ofPoint() );
	}
	
	ofxSprite* red;
	red = stage->getChildByName( "red" );
	
	if( red != NULL )
	{
		red->x = global.x;
		red->y = global.y;
	}
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
	stage->swapChildrenAt( 0, 1 );
}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

